#include "i2c_bus_init.h"


bool _i2c_bus_init( STUSB4500_ST * __st45LP , uint8_t ___i2cBusNo, uint8_t ___i2cClientAddr){
    int __rt ;

    __st45LP->i2cBusNo = ___i2cBusNo ; 
    __st45LP->i2cClientAddr = ___i2cClientAddr ;

    __rt = snprintf(__st45LP->i2cBusName, 255, "/dev/i2c-%d", ___i2cBusNo);
    if ( __rt < 10 || __rt > 12 ) {
        printf( " %s : 8181991221 : error device name <%d> <%s> \n\n", __func__, ___i2cBusNo , __st45LP->i2cBusName );
        return false ;
    }

    if ((__rt = _i2c_bus_open( __st45LP ))< 0) {
        printf( " %s : 8181991223 : error open I2C device <%d> <%s> , return <%d> \n\n", __func__, ___i2cBusNo , 
                __st45LP->i2cBusName , __rt );
        return false ;
    }
    close(__rt);

    printf( " %s : 8181991229 : succeed open I2C device <%d> <%s> , return <%d> \n\n", __func__, ___i2cBusNo , 
            __st45LP->i2cBusName , __rt );

    __st45LP-> i2cBusNo            = ___i2cBusNo ;
    __st45LP-> i2cClientAddr    = ___i2cClientAddr ;

    return true ;

} // _i2c_bus_init

int _i2c_bus_open(STUSB4500_ST * __st45LP){
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
