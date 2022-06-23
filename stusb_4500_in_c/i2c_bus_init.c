#include "i2c_bus_init.h"


char _i2c_dev_name[256] ;
bool i2c_bus_init(uint8_t ___i2cBusNo){
    int __rt ;
    __rt = snprintf(_i2c_dev_name, 255, "/dev/i2c-%d", ___i2cBusNo);
    if ( __rt < 10 || __rt > 12 ) {
        printf( " %s : 8181991221 : error device name <%d> <%s> \n", __func__, ___i2cBusNo , _i2c_dev_name );
        return false ;
    }

    if ((__rt = i2c_bus_open())< 0) {
        printf( " %s : 8181991223 : error open I2C device <%d> <%s> , return <%d> \n", __func__, ___i2cBusNo , _i2c_dev_name , __rt );
        return false ;
    }
    close(__rt);

    printf( " %s : 8181991229 : succeed open I2C device <%d> <%s> , return <%d> \n", __func__, ___i2cBusNo , _i2c_dev_name , __rt );

    return true ;

} // i2c_bus_init

int i2c_bus_open(void){
    int __rt ;
    if ((__rt = open(_i2c_dev_name, O_RDWR)) < 0) {
        printf( " %s : 881818919191 : error open I2C device <%s> , return <%d> \n", __func__, _i2c_dev_name , __rt );
    }
    return __rt ;
} // i2c_bus_open
