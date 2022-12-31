/**************************************************************
 * Class:  CSC-615-01 Fall 2022
 * Names: Faiyaz Chaudhury, Hector Magallanes,
 *        Briget Soriano, Emanuel Francis
 * Student IDs: 920273520, 921660295, 921163422, 917491268
 * GitHub Name: faiyazc
 * Group Name: 0xFF Pi
 * Project: Self Driving Car
 *
 * File: EngineDriver.c
 *
 * Description: Handles Engine init/exit, individual motor
 *              control and car movement functions.
 **************************************************************/

#include "EngineDriver.h"

struct engine ENGINE;

/**
 * Run front left wheel at specified speed and direction
 */
static void runFL(DIR dir, int speed)
{
    PCA9685_SetPwmDutyCycle(LEFT, speed, ENGINE.front_fd);
    if (dir == FORWARD)
    {
        PCA9685_SetLevel(L1, 1, ENGINE.front_fd);
        PCA9685_SetLevel(L2, 0, ENGINE.front_fd);
    }
    else
    {
        PCA9685_SetLevel(L1, 0, ENGINE.front_fd);
        PCA9685_SetLevel(L2, 1, ENGINE.front_fd);
    }
}

/**
 * Run front right wheel at specified speed and direction
 */
static void runFR(DIR dir, int speed)
{
    PCA9685_SetPwmDutyCycle(RIGHT, speed, ENGINE.front_fd);
    if (dir == FORWARD)
    {
        PCA9685_SetLevel(R1, 0, ENGINE.front_fd);
        PCA9685_SetLevel(R2, 1, ENGINE.front_fd);
    }
    else
    {
        PCA9685_SetLevel(R1, 1, ENGINE.front_fd);
        PCA9685_SetLevel(R2, 0, ENGINE.front_fd);
    }
}

/**
 * Run rear left wheel at specified speed and direction
 */
static void runRL(DIR dir, int speed)
{
    PCA9685_SetPwmDutyCycle(LEFT, speed, ENGINE.rear_fd);
    if (dir == FORWARD)
    {
        PCA9685_SetLevel(L1, 1, ENGINE.rear_fd);
        PCA9685_SetLevel(L2, 0, ENGINE.rear_fd);
    }
    else
    {
        PCA9685_SetLevel(L1, 0, ENGINE.rear_fd);
        PCA9685_SetLevel(L2, 1, ENGINE.rear_fd);
    }
}

/**
 * Run rear right wheel at specified speed and direction
 */
static void runRR(DIR dir, int speed)
{
    PCA9685_SetPwmDutyCycle(RIGHT, speed, ENGINE.rear_fd);
    if (dir == FORWARD)
    {
        PCA9685_SetLevel(R1, 0, ENGINE.rear_fd);
        PCA9685_SetLevel(R2, 1, ENGINE.rear_fd);
    }
    else
    {
        PCA9685_SetLevel(R1, 1, ENGINE.rear_fd);
        PCA9685_SetLevel(R2, 0, ENGINE.rear_fd);
    }
}

/**
 * Initialize the engine struct which holds the file descriptors
 * for each MotorHAT
 */
void engine_init(void)
{
    // system fd is somehow important
    ENGINE.sys_fd = DEV_ModuleInit();

    // top hat (addr 0x51) controls front wheels
    ENGINE.front_fd = PCA9685_Init(0x51);
    PCA9685_SetPWMFreq(100, ENGINE.front_fd);

    // bottom hat (addr 0x49) controls rear wheels
    ENGINE.rear_fd = PCA9685_Init(0x49);
    PCA9685_SetPWMFreq(100, ENGINE.rear_fd);
}

/**
 * Close all of the engine's file descriptors
 */
void engine_exit(void)
{
    DEV_ModuleExit(ENGINE.front_fd);
    DEV_ModuleExit(ENGINE.rear_fd);
    DEV_ModuleExit(ENGINE.sys_fd);
}

/**
 * Run all 4 wheels at the same speed and direction
 */
void engine_run_straight(DIR dir, int speed)
{
    runFL(dir, speed);
    runFR(dir, speed);
    runRL(dir, speed);
    runRR(dir, speed);
}

/**
 * Move to the right
 */
void engine_strafe_right(int speed)
{
    runFL(FORWARD, speed);
    runFR(BACKWARD, speed);
    runRL(BACKWARD, speed);
    runRR(FORWARD, speed);
}

/**
 * Move to the left
 */
void engine_strafe_left(int speed)
{
    runFL(BACKWARD, speed);
    runFR(FORWARD, speed);
    runRL(FORWARD, speed);
    runRR(BACKWARD, speed);
}

/**
 * Move car clock wise
 */
void engine_run_cw(int speed)
{
    runFL(FORWARD, speed);
    runFR(BACKWARD, speed - 5);
    runRL(FORWARD, speed);
    runRR(BACKWARD, speed - 5);
}

/**
 * Move car counter clock wise
 */
void engine_run_ccw(int speed)
{
    runFL(BACKWARD, speed - 5);
    runFR(FORWARD, speed);
    runRL(BACKWARD, speed - 5);
    runRR(FORWARD, speed);
}

/**
 * Stop all motors
 */
void engine_stop_all(void)
{
    // front wheels
    PCA9685_SetPwmDutyCycle(LEFT, 0, ENGINE.front_fd);
    PCA9685_SetPwmDutyCycle(RIGHT, 0, ENGINE.front_fd);

    // back wheels
    PCA9685_SetPwmDutyCycle(LEFT, 0, ENGINE.rear_fd);
    PCA9685_SetPwmDutyCycle(RIGHT, 0, ENGINE.rear_fd);
}

void engine_donut()
{
    runFL(FORWARD, 35);
    runFR(FORWARD, 35);
    runRL(FORWARD, 65);
    runRR(FORWARD, 65);
}
