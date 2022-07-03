#include "i2c_00_all.h"



// _i2c_reg_write_one_byte performs the following write I2C transaction:
//   register address (___i2cRegAddr), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool _i2c_reg_write_bytes(ST45i2cST * ___st45i2c1, uint8_t ___i2cRegAddr, uint8_t * ___buf, size_t ___len) {

    if ( ___len > 0x80 ) return false ;

    //uint8_t __buf[2] = {___i2cRegAddr, d}; // put the regAddr and dataByte into the array.
    ___st45i2c1 -> wBuf[0] = ___i2cRegAddr ;
    memcpy ( ___st45i2c1 -> wBuf + 1 , ___buf , ___len ) ;
    ___st45i2c1 -> wLEN = ___len + 1 ;
    ___st45i2c1 -> rLEN = 0 ;

    return 
        _i2c_tx1( ___st45i2c1 );

} // _i2c_reg_write_bytes

bool _i2c_reg_write_one_byte(ST45i2cST * __st45i2c2, uint8_t ___i2cRegAddr, uint8_t d) {

    uint8_t __buf[1] = {d} ;
    return 
        _i2c_reg_write_bytes( __st45i2c2, ___i2cRegAddr, __buf , 1 );

} // _i2c_reg_write_one_byte





uint8_t * _i2c_reg_read_bytes(ST45i2cST * ___st45i2c3, uint8_t ___i2cRegAddr, size_t ___len ) {

    if ( ___len > 0x80 ) return false ;

    ___st45i2c3 -> wBuf[0] = ___i2cRegAddr ;
    //memcpy ( ___st45i2c3 -> wBuf + 1 , ___buf , ___len ) ;
    ___st45i2c3 -> wLEN = 1 ;
    ___st45i2c3 -> rLEN = ___len ;

    if ( false == _i2c_tx1( ___st45i2c3 ) ) 
        return NULL ;

    return 
        (uint8_t*) ___st45i2c3 -> rBuf ; 
    //return NULL ;
} // _i2c_reg_read_bytes

uint8_t * _i2c_reg_read_one_byte(ST45i2cST * ___st45i2c4, uint8_t ___i2cRegAddr) {
    return 
        _i2c_reg_read_bytes( ___st45i2c4, ___i2cRegAddr, 1 ) ;
} // i2c_reg_read_one_byte
