/**************************************************************
 * Class:  CSC-615-01 Fall 2022
 * Names: Faiyaz Chaudhury, Hector Magallanes,
 *        Briget Soriano, Emanuel Francis
 * Student IDs: 920273520, 921660295, 921163422, 917491268
 * GitHub Name: faiyazc
 * Group Name: 0xFF Pi
 * Project: Self Driving Car
 *
 * File: main.c
 *
 * Description: Main controller for the car. After execution begins, the
 * 				program will wait for the button to be pushed to start moving
 * 				the car. Then, the car will continue to move following the line
 * 				and avoiding obstacles using LiDAR. Press Ctrl-C to stop the
 * 				car.
 **************************************************************/

#include <pigpio.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "EngineDriver.h"

#define BTN_PIN 26 // button pin
#define FL_PIN 4   // left line sensor pin
#define FR_PIN 23  // right line sensor pin

volatile int lineLeft;	// input from left line sensor
volatile int lineRight; // input from right line sensor

volatile int objFront;
volatile int objBack;
volatile int run; // car will keep running while run == 1

FILE *lidarFd;
char *line = NULL;
size_t len;

/**
 * Continuously updates the value of lineLeft as long
 * as the car continues running
 */
void *lineSensorLeftRoutine(void *ptr)
{
	while (run)
	{
		lineLeft = gpioRead(FL_PIN);
		DEV_Delay_ms(50);
	}
	return NULL;
}

/**
 * Continuously updates the value of lineRight as long
 * as the car continues running
 */
void *lineSensorRightRoutine(void *ptr)
{
	while (run)
	{
		lineRight = gpioRead(FR_PIN);
		DEV_Delay_ms(50);
	}
	return NULL;
}

/**
 * Handler to capture the Ctrl-C signal and stop
 * the car from running
 */
void stopCarHandler(int dummy)
{
	printf("--- STOP RUNNING ---\n");
	run = 0;
}

/**
 * Initialize LiDAR read to returned lidarFD.
 */
static FILE *lidarInit()
{
	lidarFd = popen("./LIDAR_drivers/ultra_simple --channel --serial /dev/ttyUSB0 115200", "r");
	if (lidarFd == NULL)
	{
		fwrite("Could not open lidarFD.\n", strlen("Could not open lidarFD.\n"), 1, stderr);
		exit(1);
	}
	return lidarFd;
}

static void lidarTerminate()
{
	pclose(lidarFd);
}

/**
 * Read lidar stdout and interpret specific specific values to handle obstacle avoidance.
 */
static void *readLidarWriteGlobal()
{
	volatile float theta, dist;

	getline(&line, &len, lidarFd);

	while (len > 0 && run)
	{
		getline(&line, &len, lidarFd);
		char *tok = strtok(line, ",");
		theta = atof(tok);
		tok = strtok(NULL, ",");
		dist = atof(tok);

		if (dist < 1.0f)
		{
			continue;
		}

		if (theta >= 19.0f && theta <= 21.0f)
		{
			if (dist <= 300.0f)
			{
				objFront = 1;
			}
			else
			{
				objFront = 0;
			}
		}
		else if (theta >= 144.0f && theta <= 146.0f)
		{
			if (dist <= 600.0f)
			{
				objBack = 1;
			}
			else
			{
				objBack = 0;
			}
		}
	}
	return NULL;
}

/**
 * Handler to avoid obstacles and get car back on the line.
 */
static void obstacleAvoidance()
{
	int strafe_speed = 30;
	int straight_speed = 20;

	printf("Object in front. Strafing left...\n");
	engine_strafe_left(strafe_speed);
	while (objFront && run)
	{
		// moving left...
	}

	printf("Object cleared. Driving forward...\n");
	engine_run_straight(FORWARD, straight_speed);
	while (!objBack && run)
	{
		// going past obstacle...
	}

	printf("Object cleared. Strafing right...\n");
	engine_strafe_right(strafe_speed);
	while (run && objBack)
	{
		// moving right...
		if (lineLeft && !lineRight)
		{
			printf("Line cleared\n");
			break;
		}
	}

	engine_stop_all();
}

int main(int argc, char **argv)
{

	run = 1; // enable car funtionality
	int speed = 25;
	// declare threads for line sensors
	pthread_t lineLeftThread, lineRightThread, lidarThread;

	if (gpioInitialise() < 0)
	{
		printf("PiGPIO initialisation failed\n");
		return 1;
	}

	// set up button and line sensor pins
	gpioSetMode(BTN_PIN, PI_INPUT);
	gpioSetPullUpDown(BTN_PIN, PI_PUD_DOWN);
	gpioSetMode(FL_PIN, PI_INPUT);
	gpioSetMode(FR_PIN, PI_INPUT);

	// initialize handler to stop car (must be done after gpioInitialise)
	signal(SIGINT, stopCarHandler);
	lidarInit();

	// initialize car's engine and start line sensor threads
	engine_init();
	pthread_create(&lineLeftThread, NULL, lineSensorLeftRoutine, NULL);
	pthread_create(&lineRightThread, NULL, lineSensorRightRoutine, NULL);
	pthread_create(&lidarThread, NULL, readLidarWriteGlobal, NULL);

	printf("Press the button to start the car!\n");
	while (gpioRead(BTN_PIN) == 0)
	{
		// wait until circuit is closed by pressing the button
		// 	to start the car
	}
	// VROOM
	while (run)
	{
		printf("Top of main.\n");
		if (objFront)
		{
			printf("Object in front.\n");
			printf("Object too close! Obstacle avoidance called.\n");
			engine_stop_all();
			obstacleAvoidance();
			printf("Back to main\n");
		}
		// go straight while line is not detected
		if (lineLeft == 0 && lineRight == 0)
		{
			printf("Go straight.\n");
			engine_run_straight(FORWARD, speed);
		}
		// rotate ccw if left sensor detects line
		else if (lineLeft == 1 && lineRight == 0)
		{
			printf("Turn ccw\n");
			engine_run_ccw(speed);
		}
		// rotate cw if right sensor detects line
		else if (lineLeft == 0 && lineRight == 1)
		{
			printf("Turn cw\n");
			engine_run_cw(speed);
		}
		// both sensors detect line (happens on tight turns)
		// keep moving until line is back in the middle
		else
		{
			DEV_Delay_ms(75);
		}
	}

	// engine_donut(); // TODO: uncomment this
	// DEV_Delay_ms(1000);

	// after stop handler signal, join threads and
	// free resources
	pthread_join(lineLeftThread, NULL);
	pthread_join(lineRightThread, NULL);
	pthread_join(lidarThread, NULL);

	gpioTerminate();
	engine_stop_all();
	engine_exit();
	lidarTerminate();

	return 0;
}
