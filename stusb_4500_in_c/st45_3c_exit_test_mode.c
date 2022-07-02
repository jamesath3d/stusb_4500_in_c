#include "st45_00_all.h"


bool _st45_exit_test_mode( ST45i2cST * ___st45I2C) {
    uint8_t __buf[2];

    // clear registers 
    __buf[0]= FTP_CUST_RST_N;
    __buf[1]= 0x00;  
    if ( false == 
            _i2c_reg_write_bytes( ___st45I2C, FTP_CTRL_0, __buf, 2) ) 
        return false ;

    // Clear Password
    if ( false == 
            _i2c_reg_write_one_byte( ___st45I2C, FTP_CUST_PASSWORD_REG, 0x00 ) )
        return false ;  

    return true ;

} // _st45_exit_test_mode
