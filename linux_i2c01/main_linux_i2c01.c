#include "i2c_00_all.h"
#include "st45_00_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

ST45i2cST _st45i2c;
//const uint8_t _st45Default[][8] = ST45default ;
extern const uint8_t _st45Default[][8] ;

//------------------------------
// _stusb4500_reset01 resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool _stusb4500_reset01( ST45i2cST * ___st45I2C ) {
    return 
        _i2c_reg_write_one_byte( ___st45I2C, 0x23, 0x01);
} // _stusb4500_reset01

uint8_t* mainX1( ST45i2cST* ___st45I2C ){
    uint8_t* __rt ;
    if ( 1 ) { // read the origin data, and try to compare with the default
        __rt = _st45_read_top( &_st45i2c );
        // _st45_nvm_read
        if ( NULL == __rt ) EXlp( NULL ) ;

        if(1) {
            _st45_cmp_buf_with_defult( __rt , "default vs read");
        } else {
            ST45_dump_buf2( __rt , "read NVM content" );
        }
    }
    return __rt ;
} // mainX1
uint8_t* mainX2( ST45i2cST* ___st45I2C , uint8_t* ___oldConfigBuf ){
    static uint8_t __Buf58[5][8] ;
    ST45config _st45config_old ;
    ST45config _st45config_new ;
    ST45config _st45config_check ;
    ST45config _st45config_default ;

    _st45_analyze_buf_to_gen_nvm_config( &_st45config_default, (uint8_t*) &(_st45Default[0][0]) ) ;
    _st45_dump_st45config(&_st45config_default, "default: ");
    _st45_analyze_buf_to_gen_nvm_config( &_st45config_old, ___oldConfigBuf ) ;
    _st45_dump_st45config(&_st45config_old, "old: ");

    // gen new config, dump to check, convert it, dump the buf, cmp the buf
    //memcpy( &_st45config_new, &_st45config_old, sizeof(ST45config));
    memcpy( &_st45config_new, &_st45config_default, sizeof(ST45config));
    _st45_gen_new_config( &_st45config_new,
            14, 7,  6,   4.35,  
            13, 8,  15,  3.45,   
            12, 9,  16,  1.55      
            );
    _st45_dump_st45config(&_st45config_new, "want:");
    _st45_convert_config_to_nvm_buf( &_st45config_new ) ; 
    ST45_dump_buf2( &(_st45config_new . buf[0][0]) , "generated new NVM content" );
    _st45_cmp_buf2( (uint8_t*) _st45config_old . buf , (uint8_t*) _st45config_new . buf , "old vs want:");

    _st45_analyze_buf_to_gen_nvm_config( &_st45config_check, (uint8_t*)_st45config_new . buf ) ;
    _st45_dump_st45config(&_st45config_check, "reAnalyze: ");

    memcpy( __Buf58 , _st45config_new . buf, 40 ) ;
    return &(__Buf58[0][0]) ;
} // mainX2

int main( int ___argc, char ** ___argv ) {
    bool __b01 ;
    uint8_t* __clp01old ;
    uint8_t* __clp02newGen ;

    __b01 = _i2c_bus_init( &_st45i2c,  STUSB4500_i2cClient_addr );
    if ( ! __b01 ) EXi( -11 ) ;

    if ( 1 ) {
        __b01 = stusb4500_read_byte_test( &_st45i2c ) ;
        if ( ! __b01 ) EXi( -13 ) ;
    }

    _i2c_tx_debug = 2 ;

    __clp01old = mainX1( &_st45i2c ) ;
    __clp02newGen = mainX2( &_st45i2c , __clp01old ) ;


    // _st45_nvm_read
    // _st45_enter_nvm_read
    // _st45_exit_test_mode
    // _i2c_tx_debug = 99 ;
    if(0) 
        _st45_nvm_write( &_st45i2c , __clp02newGen ) ;
    // CUST_WriteSector

    if(1) FP1( " reseting ... \n");
    __b01 = _stusb4500_reset01( &_st45i2c ) ;
    if ( ! __b01 ) EXi( -21 ) ;

    return 0 ; 
} // main
