#include "i2c_00_all.h"

#define i2cSysLen   55
#define i2cLineLen  51
bool _i2c_bus_init( ST45i2cST * __st45LP , uint8_t ___i2cClientAddr){
    int __rt ;
    char __i2cBusNo ; 
    char __i2cSYS[i2cSysLen] ;
    char __i2cLINE[i2cLineLen] ;

    if ( NULL == __st45LP ) EXb( false ) ;
    __st45LP -> i2cFileNO = -1 ;

    for ( __i2cBusNo = 19 ; __i2cBusNo >= 0 ; __i2cBusNo -- ) {
        __rt = snprintf( __i2cSYS, i2cSysLen, "/sys/class/i2c-adapter/i2c-%d/name", __i2cBusNo ) ;
        //FP( "len %d\n" , __rt );
        if ( 33 == __rt || 34 == __rt ) {
            __i2cSYS[__rt] = 0 ;
            FILE* __fh = fopen( __i2cSYS, "r" );
            if ( NULL != __fh ) {
                memset( __i2cLINE, 0 , i2cLineLen ) ;
                if ( NULL != fgets( __i2cLINE, i2cLineLen-1 , __fh )) {
                    // /sys/class/i2c-adapter/i2c-8/name:i2c_ch341_usb at bus 001 device 052
                    if ( 
                            NULL != strstr( __i2cLINE , "_CH341_" ) ||
                            NULL != strstr( __i2cLINE , "_ch341_" ) 
                            ) {
                        FP( "8181991221 : found %s : %s\n" , __i2cSYS , __i2cLINE );
                        fclose( __fh ) ;
                        break ;
                    }
                }
                fclose( __fh ) ;
            }
        }
    }
    if ( __i2cBusNo < 0 ) {
        FP( "\n %d \n     chmod 666 %s \n\n" , __rt , __i2cSYS ) ;
        EXb( false ) ;
    }

    __st45LP->i2cBusNo = __i2cBusNo ; 
    __rt = snprintf(__st45LP->i2cBusName, 255, "/dev/i2c-%d", __i2cBusNo);
    __st45LP->i2cClientAddr = ___i2cClientAddr ;

    if ( __rt < 10 || __rt > 12 ) {
        FP( "8181991223 : error device name <%d> <%s> \n\n", 
                __i2cBusNo , __st45LP->i2cBusName );
        EXb( false ) ;
    }

    if ((__rt = _i2c_bus_open( __st45LP ))< 0) {
        FP( "8181991225 : error open I2C device <%d> <%s> , return <%d> \n\n", 
                __i2cBusNo , __st45LP->i2cBusName , __rt );
        EXb( false );
    }

    // close(__rt);
    __st45LP -> i2cFileNO = __rt ;

    FP( "8181991229 : succeed open I2C device <%d> <%s> , return <%d> \n\n", 
            __i2cBusNo , __st45LP->i2cBusName , __rt );

    __st45LP -> i2cBusNo            = __i2cBusNo ;
    __st45LP -> i2cClientAddr    = ___i2cClientAddr ;

    return true ;

} // _i2c_bus_init

int _i2c_bus_open(ST45i2cST * __st45LP){
    int __rt1 ;
    int __rt2 ;
    if ( NULL == __st45LP ) return -1 ;
    __rt1 = __st45LP->i2cFileNO ;
    if ( __rt1 >= 0 ) {
        if(1) printf( " %s : 881818919191 : re-use the opened I2C fileNO : %d \n" , __func__, __rt1 ) ;
        return __rt1 ;
    }

    if ((__rt1 = open(__st45LP->i2cBusName, O_RDWR)) < 0) {
        printf( " %s : 881818919192 : error open I2C bus <%s> , return <%d> \n", 
                __func__, __st45LP->i2cBusName , __rt1 );
    }
    if(1) printf( " %s : 881818919193 : succeed open I2C bus <%s> , return <%d> \n", 
            __func__, __st45LP->i2cBusName , __rt1 );

    __st45LP->i2cFileNO = __rt1 ;

    int addr = __st45LP->i2cClientAddr ;          //<<<<<The I2C address of the slave
    if ((__rt2=ioctl( __rt1, I2C_SLAVE, addr)) < 0)
    {
        printf("881818919195 : Failed to set I2C slave addr:<%d>\n", addr);
        //ERROR HANDLING; you can check errno to see what went wrong
        return __rt2;
    }
    if(1) FP("881818919197 : succeed to set I2C slave addr:<%d,0x%X>\n", addr,addr);


    return __rt1 ;
} // _i2c_bus_open
