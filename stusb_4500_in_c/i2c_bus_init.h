#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>				//printf
#include <stdbool.h>            // bool
#include <stdint.h>             // uint8_t

#ifndef  I2C_BUS_INIT__
#define  I2C_BUS_INIT__

bool i2c_bus_init(uint8_t ___i2cBusNo);
int i2c_bus_open(void);

extern char _i2c_dev_name[];

#endif

