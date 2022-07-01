#include "i2c_all.h"
#include "st45_00_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

ST45i2cST _st45i2c;

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
    ST45config _st45config_old ;
    ST45config _st45config_new ;
    ST45config _st45config_check ;

    __b01 = _i2c_bus_init( &_st45i2c,  I2C_bus_NO , STUSB4500_i2cClient_addr );
    if ( ! __b01 ) return -1 ;

    //__b01 = stusb4500_read_byte_test( &_st45i2c ) ;
    if ( ! __b01 ) return -1 ;

    _i2c_tx_debug = 2 ;

    if ( 1 ) { // read the origin data, and try to compare with the default
        __clp01 =
            _st45_read_top( &_st45i2c );
        if ( NULL == __clp01 ) return -1 ;

        if(1) {
            __b01 = 
                _st45_cmp_buf_with_defult( __clp01 , "default vs read");
        } else {
            ST45_dump_buf2( __clp01 , "read NVM content" );
        }
    }

    _st45_analyze_buf_to_gen_nvm_config( &_st45config_old, __clp01 ) ;
    _st45_dump_st45config(&_st45config_old, "old: ");

    // gen new config, dump to check, convert it, dump the buf, cmp the buf
    memcpy( &_st45config_new, &_st45config_old, sizeof(ST45config));
    _st45_gen_new_config( &_st45config_new,
            14, 12,  5,   3.25,  
            13, 13,  12,  1.5,   
            12, 14,  15,  1.2      
            );
    _st45_dump_st45config(&_st45config_new, "want:");
    _st45_convert_config_to_nvm_buf( &_st45config_new ) ; 
    ST45_dump_buf2( &(_st45config_new . buf[0][0]) , "generated new NVM content" );
    _st45_cmp_buf2( (uint8_t*) _st45config_old . buf , (uint8_t*) _st45config_new . buf , "old vs want:");

    _st45_analyze_buf_to_gen_nvm_config( &_st45config_check, (uint8_t*)_st45config_new . buf ) ;
    _st45_dump_st45config(&_st45config_check, "reAnalyze: ");
    /*


       __st45analyze =
       _st45_analyze_buf_to_gen_nvm_config( __clp01 ) ;
       _st45_dump_st45config(__st45analyze);

       _i2c_tx_debug = 2 ;

       __b01 = _stusb4500_reset01( &_st45i2c ) ;
       if ( ! __b01 ) return -1 ;

*/

    return 0 ; 
} // main
