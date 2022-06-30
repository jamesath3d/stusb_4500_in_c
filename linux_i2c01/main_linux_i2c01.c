#include "i2c_all.h"
#include "st45_00_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

ST45i2cST _stusb4500_St;

//------------------------------
// _stusb4500_reset01 resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool _stusb4500_reset01( ST45i2cST * __st45LP ) {
    return 
        _i2c_reg_write_one_byte( __st45LP, 0x23, 0x01);
} // _stusb4500_reset01


int main( int ___argc, char ** ___argv ) {
    bool __b01 ;
    uint8_t* __clp01 ;
    ST45config* __st45analyze ;

    __b01 = _i2c_bus_init( &_stusb4500_St,  I2C_bus_NO , STUSB4500_i2cClient_addr );
    if ( ! __b01 ) return -1 ;

    //__b01 = stusb4500_read_byte_test( &_stusb4500_St ) ;
    if ( ! __b01 ) return -1 ;

    _i2c_tx_debug = 2 ;

    if ( 1 ) { // read the origin data, and try to compare with the default
        __clp01 =
            _st45_read_top( &_stusb4500_St );
        if ( NULL == __clp01 ) return -1 ;

        if(1) {
            __b01 = 
                _st45_comp_buf_with_defult( __clp01 );
        } else {
            ST45_dump_buf2( __clp01 , "read NVM content" );
        }
    }

    __st45analyze =
        _st45_analyze_nvm( __clp01 ) ;
    _st45_dump_st45config(__st45analyze);


    __clp01 = 
        _st45_gen_new_config( 
                14, 12, 33.25,
                13, 13, 1.5,    9,
                12, 14, 1,      20
                );
    ST45_dump_buf2( __clp01 , "generated new NVM content" );

    __st45analyze =
        _st45_analyze_nvm( __clp01 ) ;
    _st45_dump_st45config(__st45analyze);

    _i2c_tx_debug = 2 ;

    __b01 = _stusb4500_reset01( &_stusb4500_St ) ;
    if ( ! __b01 ) return -1 ;

    return 0 ; 
} // main
