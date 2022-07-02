#include "st45_00_all.h"


int _st45_nvm_write( ST45i2cST * ___st45I2C, uint8_t * ___wBufTo ) {
    static char __wBuf[5][8]; // must read all 40 bytes.

    if ( NULL == ___st45I2C || NULL == ___wBufTo ) {
        FP1( "818328191 : parameter NULL \n" );
    }

    memcpy(__wBuf,___wBufTo,sizeof(__wBuf));

    if ( 3 != _st45_enter_nvm_write( ___st45I2C ) ) EXi( -1 ) ;

    if ( 8 != _st45_nvm_write_8_bytes(___st45I2C, 0, &(__wBuf[0][0])) ) EXi( -1 ) ;

    /*
    if ( 3 != _st45_enter_nvm_read( ___st45I2C ) ) return -1;

    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 0, &(__wBuf[0][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 1, &(__wBuf[1][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 2, &(__wBuf[2][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 3, &(__wBuf[3][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 4, &(__wBuf[4][0])) ) return -1;

    */

    if ( false == 
            _st45_exit_test_mode(___st45I2C) ) EXi( -41 ) ;


    return 0; //OK
} // _st45_nvm_write
