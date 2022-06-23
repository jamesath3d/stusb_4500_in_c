#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>				//printf
#include <stdbool.h>            // bool
#include <stdint.h>             // uint8_t

#include "i2c_tx.h"
#include "i2c_reg_write.h"


//------------------------------
// stusb4500_reset resets the STUSB4500. This also results in loss of
// power to the entire board while STUSB4500 boots up again, effectively
// resetting the uC as well.
bool stusb4500_reset() {
    enum { addr = 0x28 };

    i2c_reg_write_one_byte(addr, 0x23, 0x01);
    // i2c_tx1(___i2cClientAddr, __buf, 2, NULL, 0);

    return true ;
} // stusb4500_reset

int main( int ___argc, char ** ___argv ) {
    //return 0 ;
    return (true==stusb4500_reset())?0:-1 ;
} // main
