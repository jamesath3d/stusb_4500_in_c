#include "st45_00_all.h"


ST45config* _st45_convert_config_to_nvm_buf(ST45config* ___st45config) {
    uint8_t __u01 ;
    if ( NULL == ___st45config ) return NULL ;

    ___st45config -> buf[3][2] &= (~0b110) ;
    __u01 = 0b110 & ((___st45config -> pdoAmount)<<1) ;
    ___st45config -> buf[3][2] |= __u01;

    return ___st45config ;
} // _st45_convert_config_to_nvm_buf
