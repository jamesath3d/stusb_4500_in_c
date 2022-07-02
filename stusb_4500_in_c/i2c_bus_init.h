
#ifndef  I2C_BUS_INIT__
#define  I2C_BUS_INIT__

#include "i2c_all.h"


bool _i2c_bus_init( ST45i2cST * __st45LP , uint8_t ___i2cClientAddr) ;
int  _i2c_bus_open(struct ST45i2cST * __st45LP);

extern char _i2c_dev_name[];

#endif

