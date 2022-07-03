
#ifndef  I2C_23_REG_WRITE_H__
#define  I2C_23_REG_WRITE_H__

#include "i2c_00_all.h"

bool _i2c_reg_write_bytes(ST45i2cST * __st45LP, uint8_t __i2cRegAddr, uint8_t * ___buf, size_t ___len) ;
bool _i2c_reg_write_one_byte(ST45i2cST * __st45LP, uint8_t __i2cRegAddr, uint8_t d) ;

uint8_t * _i2c_reg_read_one_byte(ST45i2cST * __st45LP, uint8_t __i2cRegAddr) ;
uint8_t * _i2c_reg_read_bytes(ST45i2cST * ___st45LP, uint8_t ___i2cRegAddr, size_t ___len ) ;

#endif

