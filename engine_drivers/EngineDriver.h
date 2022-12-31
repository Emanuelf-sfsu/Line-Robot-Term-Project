/**************************************************************
* Class:  CSC-615-01 Fall 2022
* Names: Faiyaz Chaudhury, Hector Magallanes,
*        Briget Soriano, Emanuel Francis
* Student IDs: 920273520, 921660295, 921163422, 917491268
* GitHub Name: faiyazc
* Group Name: 0xFF Pi
* Project: Self Driving Car
*
* File: EngineDriver.h
*
* Description: Header file for Engine functions.
**************************************************************/

#include "DEV_Config.h"
#include "PCA9685.h"

#define LEFT        PCA_CHANNEL_0
#define L1        PCA_CHANNEL_1
#define L2        PCA_CHANNEL_2
#define RIGHT       PCA_CHANNEL_5
#define R1        PCA_CHANNEL_3
#define R2        PCA_CHANNEL_4

typedef enum {
    FORWARD  = 1,
    BACKWARD = 0,
} DIR;

/**
* This struct contains the file descriptors for each MotorHAT
* These different file descriptors allow us to control either
* the front or rear wheels
*/
typedef struct engine {
    int sys_fd;
	int front_fd;
    int rear_fd;
} engine;

void engine_init(void);
void engine_exit(void);
void engine_run_straight(DIR dir, int speed);
void engine_strafe_right(int speed);
void engine_run_cw(int speed);
void engine_run_ccw(int speed);
void engine_strafe_left(int speed);
void engine_stop_all(void);
void engine_donut(void);
