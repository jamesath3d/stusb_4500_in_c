#include "i2c_tx.h"

//#define LinuxI2C_device "/dev/i2c-8"
//#define LinuxI2C_i2cAddr 0x28

// _i2c_tx1 returns true of transaction is successful, false otherwise.
// An I2C transaction involves one of the following:
//   a. a write (where wLen > 0 and rLen == 0)
//   b. a read  (where wLen == 0 and rLen > 0)
//   c. a write followed by a read (where wLen > 0 and rLen > 0)
// Data to be written is read from w. Data to be read is stored in r.

uint8_t _i2c_tx_debug = 0 ;

bool _i2c_tx2(ST45i2cST * ___st45i2c1){
    ssize_t __wLen ;
    ssize_t __rLen ;
    ssize_t __wRT ;
    ssize_t __rRT ;

    if ( ___st45i2c1->wLEN == 0 && ___st45i2c1->rLEN == 0) {
        return true;
    }
    if ( ___st45i2c1->wLEN > 0) {
        __wLen = ___st45i2c1->wLEN ;
        __wRT = write( ___st45i2c1->i2cFileNO, ___st45i2c1->wBuf, __wLen) ;
        if ( __wRT != __wLen ) {
            if(_i2c_tx_debug>10) printf( " 883918181 : write error   ") ;
        } else {
            if(_i2c_tx_debug>10) printf( " 883918182 : write succeed ") ;
        }
        if(_i2c_tx_debug>10) printf( " <%ld> vs <%ld> : ", __wLen , __wRT );
        for ( uint8_t __ii=0 ; __ii < __wLen ; __ii ++ ) {
            if ( 1 == __ii )  if(_i2c_tx_debug>10) printf( " :" );
            if(_i2c_tx_debug>10) printf( " %02hhX" , ___st45i2c1->wBuf[__ii] );
        }
        if ( 1 == __wLen ) if(_i2c_tx_debug>10) printf( " <--- Write addre only." );
        if(_i2c_tx_debug>10) printf( "\n" );
    }
    if ( ___st45i2c1->rLEN > 0) {
        __rLen = ___st45i2c1->rLEN ;
        __rRT = read(___st45i2c1->i2cFileNO, ___st45i2c1->rBuf, __rLen) ;
        if ( __rRT != __rLen ) {
            if(_i2c_tx_debug>10) printf( " 883918183 # read  error   ");
        } else {
            if(_i2c_tx_debug>10) printf( " 883918184 # read  succeed ");
        }
        if(_i2c_tx_debug>10) printf( " <%ld> vs <%ld> : ", __rLen , __rRT );
        if(_i2c_tx_debug>10) printf( " %02hhX=#=" , ___st45i2c1->wBuf[0] );
        for ( uint8_t __ii=0 ; __ii < __rLen ; __ii ++ ) {
            if(_i2c_tx_debug>10) printf( " %02hhX" , ___st45i2c1->rBuf[__ii] );
        }
        if(_i2c_tx_debug>10) printf( "\n" );

    }

    return true ;
} // _i2c_tx2

bool _i2c_tx1(ST45i2cST * ___st45i2c2){
    //int length;
    //unsigned char buffer[60] = {0};
    //int __fi_i2c;


    // if no read, no write, NOTHING need to be done.
    if (___st45i2c2->wLEN == 0 && ___st45i2c2->rLEN == 0) { 
        return true;
    }

    //----- OPEN THE I2C BUS -----
    if (___st45i2c2->i2cFileNO < 0) _i2c_bus_open(___st45i2c2) ;
    if (___st45i2c2->i2cFileNO < 0) 
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        FP1(" 831919911 :  Failed to open the i2c bus. \n\n" );
        return false;
    }

    bool __rt1 = _i2c_tx2(___st45i2c2);

    //bool __rt2 = (0 == close( ___st45i2c2->i2cFileNO ))?true:false;
    bool __rt2 = true ;

    if ( 0 ) printf(" 831919915 : Function %s return <%d> <%d>\n", __func__ , __rt1, __rt2 );

    return __rt1 && __rt2;
    //return true;
} // _i2c_tx1

