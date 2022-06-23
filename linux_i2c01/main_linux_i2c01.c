#include "i2c_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

STUSB4500_ST _stusb4500_St;

//------------------------------
// _stusb4500_reset01 resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool _stusb4500_reset01( STUSB4500_ST * __st45LP ) {

    _i2c_reg_write_one_byte( __st45LP , 0x23, 0x01);
    // _i2c_tx1(__buf, 2, NULL, 0);

    return true ;
} // _stusb4500_reset01

bool stusb4500_read_byte_test(void){
    //i2c_reg_read_bytes(0x23, 0x01);

    return true ;
} // stusb4500_read_byte_test

int main( int ___argc, char ** ___argv ) {
    //return 0 ;
    bool __b01 = true ;
    bool __b09 = true ;

    __b01 = _i2c_bus_init( &_stusb4500_St,  I2C_bus_NO , STUSB4500_i2cClient_addr );

    __b09 = _stusb4500_reset01( &_stusb4500_St ) ;
    //b01 = stusb4500_read_byte_test() ;

    return __b01 && __b09 ; 
} // main
