#include "i2c_tx.h"
#include "i2c_reg_write.h"



// _i2c_reg_write_one_byte performs the following write I2C transaction:
//   register address (___i2cRegAddr), register value (d)
// This is a common way of writing into I2C device registers.
// Returns true if successful, false otherwise.
bool _i2c_reg_write_bytes(ST45i2cST * ___st45LP, char ___i2cRegAddr, char * ___buf, size_t ___len) {

    if ( ___len > 0x80 ) return false ;

    //uint8_t __buf[2] = {___i2cRegAddr, d}; // put the regAddr and dataByte into the array.
    ___st45LP -> wBuf[0] = ___i2cRegAddr ;
    memcpy ( ___st45LP -> wBuf + 1 , ___buf , ___len ) ;
    ___st45LP -> wLEN = ___len + 1 ;
    ___st45LP -> rLEN = 0 ;

    return 
        _i2c_tx1( ___st45LP );

} // _i2c_reg_write_bytes

bool _i2c_reg_write_one_byte(ST45i2cST * ___st45LP, char ___i2cRegAddr, char d) {

    char __buf[1] = {d} ;
    return 
        _i2c_reg_write_bytes( ___st45LP, ___i2cRegAddr, __buf , 1 );

} // _i2c_reg_write_one_byte





char * i2c_reg_read_bytes(ST45i2cST * ___st45LP, char ___i2cRegAddr, size_t ___len ) {

    if ( ___len > 0x80 ) return false ;

    ___st45LP -> wBuf[0] = ___i2cRegAddr ;
    //memcpy ( ___st45LP -> wBuf + 1 , ___buf , ___len ) ;
    ___st45LP -> wLEN = 1 ;
    ___st45LP -> rLEN = ___len ;

    if ( false == _i2c_tx1( ___st45LP ) ) 
        return NULL ;

    return 
        ___st45LP -> rBuf ; 
    //return NULL ;
} // i2c_reg_read_bytes

char * i2c_reg_read_one_byte(ST45i2cST * ___st45LP, char ___i2cRegAddr) {
    return 
        i2c_reg_read_bytes( ___st45LP, ___i2cRegAddr, 1 ) ;
} // i2c_reg_read_one_byte
