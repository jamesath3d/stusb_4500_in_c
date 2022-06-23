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
bool _i2c_tx2(int ___fi_i2c, uint8_t *___wBuf, uint8_t ___wLen, uint8_t *___rBuf, uint8_t ___rLen) {
    ssize_t __wLen ;
    ssize_t __rLen ;
    ssize_t __wRT ;
    ssize_t __rRT ;

    if ( ___wLen == 0 && ___rLen == 0) {
        return true;
    }
    if ( ___wLen > 0) {
        __wLen = ___wLen ;
        __wRT = write(___fi_i2c, ___wBuf, __wLen) ;
        if ( __wRT != __wLen ) {
            printf( " 883918181 : write error <%ld> vs <%ld> \n", __wLen , __wRT );
        }
    }
    if ( ___rLen > 0) {
        __rLen = ___rLen ;
        __rRT = read(___fi_i2c, ___rBuf, __rLen) ;
        if ( __rRT != __rLen ) {
            printf( " 883918183 : read error <%ld> vs <%ld> \n", __rLen , __rRT );
        }

    }

    return true ;
} // _i2c_tx2

bool _i2c_tx1(uint8_t *___wBuf, uint8_t ___wLen, uint8_t *___rBuf, uint8_t ___rLen) {
    //int length;
    //unsigned char buffer[60] = {0};
    //int __fi_i2c;


    if (___wLen == 0 && ___rLen == 0) { // no read, no write, so nothing need to be done.
        return true;
    }

    //----- OPEN THE I2C BUS -----
    if ((_stusb4500_St.i2cBusFD = _i2c_bus_open()) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf(" 831919911 :  Failed to open the i2c bus. \n\n" );
        return false;
    }

    /*
       int i2c_device_addr = LinuxI2C_i2cAddr ;        //<<<<<The I2C address of the slave
       if (ioctl(_stusb4500_St.i2cBusFD, I2C_SLAVE, i2c_device_addr) < 0)
       {
       printf("Failed to acquire bus access and/or talk to slave at i2c address <0x%x>.\n", i2c_device_addr );
    //ERROR HANDLING; you can check errno to see what went wrong
    return false;
    }

    printf("Succeed open <%s> bus at i2c address <0x%x>, and get file handle <0x%x>.\n", 
            filename, i2c_device_addr , _stusb4500_St.i2cBusFD);
    */

    bool __rt1 = _i2c_tx2(_stusb4500_St.i2cBusFD, ___wBuf, ___wLen, ___rBuf, ___rLen) ;

    bool __rt2 = (0 == close( _stusb4500_St.i2cBusFD ))?true:false;

    printf(" Function %s return <%d> <%d>\n", __func__ , __rt1, __rt2 );

    return __rt1 && __rt2;
    //return true;
} // _i2c_tx1

