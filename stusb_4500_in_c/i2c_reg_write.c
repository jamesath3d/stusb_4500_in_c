#include "i2c_tx.h"
#include "i2c_reg_write.h"



// i2c_reg_write performs the following write I2C transaction:
//   register address (reg), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool i2c_reg_write(uint8_t addr, uint8_t reg, uint8_t d) {
  uint8_t buf[] = {reg, d};
  return i2c_tx(addr, buf, 2, NULL, 0);
} // i2c_reg_write

