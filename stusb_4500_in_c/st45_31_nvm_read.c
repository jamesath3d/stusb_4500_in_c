#include "st45_00_all.h"

// learn from STUSB4500_USBPD_CUST_NVM_API.c line 236
int _st45_nvm_read( ST45i2cST * ___st45I2C, uint8_t * ___rBufStore, int __SectorsLength) {
    static char __rBuf[5][8]; // must read all 40 bytes.

    memset(__rBuf,0,sizeof(__rBuf));

    if(__SectorsLength != ST45_NVM_SIZE) //error, input buffer too small
        EXi( -21 ) ;

    if ( 3 != _st45_enter_nvm_read( ___st45I2C ) ) EXi( -31 ) ;

    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 0, &(__rBuf[0][0])) ) EXi( -32 );
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 1, &(__rBuf[1][0])) ) EXi( -33 );
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 2, &(__rBuf[2][0])) ) EXi( -34 );
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 3, &(__rBuf[3][0])) ) EXi( -35 );
    if ( 8 != _st45_nvm_read_8_bytes(___st45I2C, 4, &(__rBuf[4][0])) ) EXi( -36 );

    if ( false == 
            _st45_exit_test_mode(___st45I2C) ) EXi( -41 ) ;

    memcpy( ___rBufStore, __rBuf , sizeof( __rBuf ));

    return 0; //OK
} // _st45_nvm_read
