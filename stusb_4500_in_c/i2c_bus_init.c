#include "i2c_bus_init.h"


struct STUSB4500_ST _stusb4500_St;

bool _i2c_bus_init(uint8_t ___i2cBusNo, uint8_t ___i2cClientAddr){
    int __rt ;
    __rt = snprintf(_stusb4500_St.i2cBusName, 255, "/dev/i2c-%d", ___i2cBusNo);
    if ( __rt < 10 || __rt > 12 ) {
        printf( " %s : 8181991221 : error device name <%d> <%s> \n\n", __func__, ___i2cBusNo , _stusb4500_St.i2cBusName );
        return false ;
    }

    if ((__rt = _i2c_bus_open())< 0) {
        printf( " %s : 8181991223 : error open I2C device <%d> <%s> , return <%d> \n\n", __func__, ___i2cBusNo , 
                _stusb4500_St.i2cBusName , __rt );
        return false ;
    }
    close(__rt);

    printf( " %s : 8181991229 : succeed open I2C device <%d> <%s> , return <%d> \n\n", __func__, ___i2cBusNo , 
            _stusb4500_St.i2cBusName , __rt );

    _stusb4500_St . i2cBusNo            = ___i2cBusNo ;
    _stusb4500_St . i2cClientAddress    = ___i2cClientAddr ;

    return true ;

} // _i2c_bus_init

int _i2c_bus_open(void){
    int __rt ;
    if ((__rt = open(_stusb4500_St.i2cBusName, O_RDWR)) < 0) {
        printf( " %s : 881818919191 : error open I2C device <%s> , return <%d> \n", __func__, 
                _stusb4500_St.i2cBusName , __rt );
    }
    return __rt ;
} // _i2c_bus_open
