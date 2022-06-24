#include "i2c_tx.h"
#include "i2c_reg_write.h"



// _i2c_reg_write_one_byte performs the following write I2C transaction:
//   register address (__i2cRegAddr), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool _i2c_reg_write_bytes(STUSB4500_ST * __st45LP, char __i2cRegAddr, char * ___buf, size_t ___len) {

    if ( ___len > 0x80 ) return false ;

    size_t __len = ___len ;

  //uint8_t __buf[2] = {__i2cRegAddr, d}; // put the regAddr and dataByte into the array.
  __st45LP -> wBuf[0] = __i2cRegAddr ;
  memcpy ( __st45LP -> wBuf + 1 , ___buf , __len ) ;
  __st45LP -> wLEN = ___len + 1 ;
  __st45LP -> rLEN = 0 ;

  return 
      _i2c_tx1( __st45LP );

} // _i2c_reg_write_bytes

bool _i2c_reg_write_one_byte(STUSB4500_ST * __st45LP, char __i2cRegAddr, char d) {

  char __buf[1] = {d} ;
  return 
      _i2c_reg_write_bytes( __st45LP, __i2cRegAddr, __buf , 1 );

} // _i2c_reg_write_one_byte

char * i2c_reg_read_one_byte(STUSB4500_ST * __st45LP, char __i2cRegAddr) {
    return __st45LP -> rBuf ; 
    //return NULL ;
} // i2c_reg_read_one_byte
