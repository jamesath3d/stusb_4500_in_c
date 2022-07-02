#include "i2c_bus_init.h"

#define i2cSysLen   55
#define i2cLineLen  51
bool _i2c_bus_init( ST45i2cST * __st45LP , uint8_t ___i2cClientAddr){
    int __rt ;
    char __i2cBusNo ; 
    char __i2cSYS[i2cSysLen] ;
    char __i2cLINE[i2cLineLen] ;

    for ( __i2cBusNo = 19 ; __i2cBusNo >= 0 ; __i2cBusNo -- ) {
        __rt = snprintf( __i2cSYS, i2cSysLen, "/sys/class/i2c-adapter/i2c-%d/name", __i2cBusNo ) ;
        //printf( "len %d\n" , __rt );
        if ( 33 == __rt || 34 == __rt ) {
            __i2cSYS[__rt] = 0 ;
            FILE* __fh = fopen( __i2cSYS, "r" );
            if ( NULL != __fh ) {
                memset( __i2cLINE, 0 , i2cLineLen ) ;
                if ( NULL != fgets( __i2cLINE, i2cLineLen-1 , __fh )) {
                    if ( NULL != strstr( __i2cLINE , "CH341" ) ) {
                        printf( "found %s : %s\n" , __i2cSYS , __i2cLINE );
                        fclose( __fh ) ;
                        break ;
                    }
                }
                fclose( __fh ) ;
            }
        }
    }
    if ( __i2cBusNo < 0 ) {
        return false ;
    }

    __st45LP->i2cBusNo = __i2cBusNo ; 
    __rt = snprintf(__st45LP->i2cBusName, 255, "/dev/i2c-%d", __i2cBusNo);
    __st45LP->i2cClientAddr = ___i2cClientAddr ;

    if ( __rt < 10 || __rt > 12 ) {
        printf( " %s : 8181991221 : error device name <%d> <%s> \n\n", __func__, __i2cBusNo , __st45LP->i2cBusName );
        return false ;
    }

    if ((__rt = _i2c_bus_open( __st45LP ))< 0) {
        printf( " %s : 8181991223 : error open I2C device <%d> <%s> , return <%d> \n\n", __func__, __i2cBusNo , 
                __st45LP->i2cBusName , __rt );
        return false ;
    }
    close(__rt);

    printf( " %s : 8181991229 : succeed open I2C device <%d> <%s> , return <%d> \n\n", __func__, __i2cBusNo , 
            __st45LP->i2cBusName , __rt );

    __st45LP-> i2cBusNo            = __i2cBusNo ;
    __st45LP-> i2cClientAddr    = ___i2cClientAddr ;

    return true ;

} // _i2c_bus_init

int _i2c_bus_open(ST45i2cST * __st45LP){
    int __rt ;
    if ((__rt = open(__st45LP->i2cBusName, O_RDWR)) < 0) {
        printf( " %s : 881818919191 : error open I2C device <%s> , return <%d> \n", __func__, 
                __st45LP->i2cBusName , __rt );
    }


    int addr = __st45LP->i2cClientAddr ;          //<<<<<The I2C address of the slave
    if ((__rt=ioctl( __rt, I2C_SLAVE, addr)) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave.<%d>\n", addr);
        //ERROR HANDLING; you can check errno to see what went wrong
        return __rt;
    }

    __st45LP->i2cBusFD = __rt ;

    return __rt ;
} // _i2c_bus_open
