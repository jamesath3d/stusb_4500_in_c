
#ifndef  I2C_ALL_H__
#define  I2C_ALL_H__

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>				//printf
#include <stdbool.h>            // bool
#include <stdint.h>             // uint8_t

struct STUSB4500_ST {
    uint8_t     i2cBusNo ;
    char        i2cBusName[256] ;
    int         i2cBusFD ;
    uint8_t     i2cClientAddress ;
} ;
extern struct STUSB4500_ST _stusb4500_St;

#include "i2c_bus_init.h"
#include "i2c_tx.h"
#include "i2c_reg_write.h"

#endif

