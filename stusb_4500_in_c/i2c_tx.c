#include "i2c_tx.h"

//#define LinuxI2C_device "/dev/i2c-8"
//#define LinuxI2C_i2cAddr 0x28

// _i2c_tx1 returns true of transaction is successful, false otherwise.
// An I2C transaction involves one of the following:
//   a. a write (where wLen > 0 and rLen == 0)
//   b. a read  (where wLen == 0 and rLen > 0)
//   c. a write followed by a read (where wLen > 0 and rLen > 0)
// Data to be written is read from w. Data to be read is stored in r.

//#define Rlen 0x80
#define Rlen 0x01
bool _i2c_tx2(STUSB4500_ST * __st45LP){
    ssize_t __wLen ;
    ssize_t __rLen ;
    ssize_t __wRT ;
    ssize_t __rRT ;

    if ( __st45LP->wLEN == 0 && __st45LP->rLEN == 0) {
        return true;
    }
    if ( __st45LP->wLEN > 0) {
        __wLen = __st45LP->wLEN ;
        __wRT = write( __st45LP->i2cBusFD, __st45LP->wBuf, __wLen) ;
        if ( __wRT != __wLen ) {
            printf( " 883918181 : write error <%ld> vs <%ld> \n", __wLen , __wRT );
        }
    }
    if ( __st45LP->rLEN > 0) {
        __rLen = __st45LP->rLEN ;
        __rRT = read(__st45LP->i2cBusFD, __st45LP->rBuf, __rLen) ;
        if ( __rRT != __rLen ) {
            printf( " 883918183 : read error <%ld> vs <%ld> \n", __rLen , __rRT );
        }

    }

    return true ;
} // _i2c_tx2

bool _i2c_tx1(STUSB4500_ST * __st45LP){
    //int length;
    //unsigned char buffer[60] = {0};
    //int __fi_i2c;


    if (__st45LP->wLEN == 0 && __st45LP->rLEN == 0) { // no read, no write, so nothing need to be done.
        return true;
    }

    //----- OPEN THE I2C BUS -----
    if ((__st45LP->i2cBusFD = _i2c_bus_open(__st45LP)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf(" 831919911 :  Failed to open the i2c bus. \n\n" );
        return false;
    }

    bool __rt1 = _i2c_tx2(__st45LP);

    bool __rt2 = (0 == close( __st45LP->i2cBusFD ))?true:false;

    printf(" Function %s return <%d> <%d>\n", __func__ , __rt1, __rt2 );

    return __rt1 && __rt2;
    //return true;
} // _i2c_tx1

