#include "st45_00_all.h"


ST45config* _st45_convert_config_to_nvm_buf(ST45config* ___st45config) {
    uint16_t __u01 ;
    //uint16_t __u02 ;
    if ( NULL == ___st45config ) return NULL ;

    ___st45config -> buf[3][2] &= (~0b110) ;
    __u01 = 0b110 & ((___st45config -> pdoAmount)<<1) ;
    ___st45config -> buf[3][2] |= __u01;

    //PDO1v - fixed at 5V and is unable to change, ignore
    //PDO2v : Load voltage (10-bit)
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    // -bit 0:1 - sector 4, byte 0, bits 6:7


    return ___st45config ;
} // _st45_convert_config_to_nvm_buf
