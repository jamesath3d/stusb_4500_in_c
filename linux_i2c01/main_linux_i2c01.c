#include "i2c_all.h"

#define STUSB4500_i2cClient_addr 0x28
#define I2C_bus_NO 8

//------------------------------
// stusb4500_reset01 resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool stusb4500_reset01() {

    i2c_reg_write_one_byte(STUSB4500_i2cClient_addr, 0x23, 0x01);
    // i2c_tx1(___i2cClientAddr, __buf, 2, NULL, 0);

    return true ;
} // stusb4500_reset01

bool stusb4500_read_byte_test(void){
    //i2c_reg_read_bytes(STUSB4500_i2cClient_addr, 0x23, 0x01);

    return true ;
} // stusb4500_read_byte_test

int main( int ___argc, char ** ___argv ) {
    //return 0 ;
    bool __b01 = true ;
    bool __b09 = true ;

    __b01 = i2c_bus_init( I2C_bus_NO );

    __b09 = stusb4500_reset01() ;
    //b01 = stusb4500_read_byte_test() ;

    return __b01 && __b09 ; 
} // main
