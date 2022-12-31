#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_
/***********************************************************************************************************************
			------------------------------------------------------------------------
			|\\\																///|
			|\\\					Hardware interface							///|
			------------------------------------------------------------------------
***********************************************************************************************************************/
#include "sysfs_gpio.h"
#include "dev_hardware_i2c.h"

#include <stdint.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t


extern int INT_PIN;//4
/*------------------------------------------------------------------------------------------------------*/
int DEV_ModuleInit(void);
void DEV_ModuleExit(int fd);

int DEV_I2C_Init(uint8_t Add);
void I2C_Write_Byte(uint8_t Cmd, uint8_t value, int fd);
int I2C_Read_Byte(uint8_t Cmd, int fd);

void DEV_GPIO_Mode(UWORD Pin, UWORD Mode);

void DEV_Delay_ms(UDOUBLE xms);

#endif
