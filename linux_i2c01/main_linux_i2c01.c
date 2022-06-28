#include "i2c_all.h"
#include "st45_00_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

STUSB4500_ST _stusb4500_St;

//------------------------------
// _stusb4500_reset01 resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool _stusb4500_reset01( STUSB4500_ST * __st45LP ) {
    return 
        _i2c_reg_write_one_byte( __st45LP, 0x23, 0x01);
} // _stusb4500_reset01


int main( int ___argc, char ** ___argv ) {
    bool __b01 ;
    char* __clp01 ;

    __b01 = _i2c_bus_init( &_stusb4500_St,  I2C_bus_NO , STUSB4500_i2cClient_addr );
    if ( ! __b01 ) return -1 ;

    //__b01 = stusb4500_read_byte_test( &_stusb4500_St ) ;
    if ( ! __b01 ) return -1 ;

    _i2c_tx_debug = false ;

    __clp01 =
        _st45_read_top( &_stusb4500_St );
    if ( NULL == __clp01 ) return -1 ;

    __b01 = 
        _st45_comp_buf_with_defult( &_stusb4500_St , __clp01 );

    _i2c_tx_debug = true ;

    __b01 = _stusb4500_reset01( &_stusb4500_St ) ;
    if ( ! __b01 ) return -1 ;

    return 0 ; 
} // main
