#include "i2c_tx.h"
#include "i2c_reg_write.h"



// _i2c_reg_write_one_byte performs the following write I2C transaction:
//   register address (__i2cRegAddr), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool _i2c_reg_write_one_byte(STUSB4500_ST * __st45LP, uint8_t __i2cRegAddr, uint8_t d) {

  //uint8_t __buf[2] = {__i2cRegAddr, d}; // put the regAddr and dataByte into the array.
  __st45LP -> wBuf[0] = __i2cRegAddr ;
  __st45LP -> wBuf[1] = d ;
  __st45LP -> wLEN = 2 ;
  __st45LP -> rLEN = 0 ;

  return 
      _i2c_tx1( __st45LP );

} // _i2c_reg_write_one_byte

