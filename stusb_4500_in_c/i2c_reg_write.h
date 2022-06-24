
#ifndef  I2C_REG_WRITE_H__
#define  I2C_REG_WRITE_H__

#include "i2c_all.h"

bool _i2c_reg_write_bytes(STUSB4500_ST * __st45LP, char __i2cRegAddr, char * ___buf, size_t ___len) ;
bool _i2c_reg_write_one_byte(STUSB4500_ST * __st45LP, char __i2cRegAddr, char d) ;

char * i2c_reg_read_one_byte(STUSB4500_ST * __st45LP, char __i2cRegAddr) ;
char * i2c_reg_read_bytes(STUSB4500_ST * ___st45LP, char ___i2cRegAddr, size_t ___len ) ;

#endif

