#include "i2c_tx.h"
#include "i2c_reg_write.h"



// i2c_reg_write_one_byte performs the following write I2C transaction:
//   register address (__i2cRegAddr), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool i2c_reg_write_one_byte(uint8_t ___i2cClientAddr, uint8_t __i2cRegAddr, uint8_t d) {

  uint8_t __buf[2] = {__i2cRegAddr, d}; // put the regAddr and dataByte into the array.

  return 
      i2c_tx1(___i2cClientAddr, __buf, 2, NULL, 0);

} // i2c_reg_write_one_byte

