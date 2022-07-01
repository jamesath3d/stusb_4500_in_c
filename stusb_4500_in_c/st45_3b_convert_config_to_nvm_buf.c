#include "st45_00_all.h"


ST45config* _st45_convert_config_to_nvm_buf(ST45config* ___st45config) {
    uint16_t __u01 ;
    uint16_t __u02 ;
    uint8_t  __u81 ;
    uint8_t  __u82 ;
    if ( NULL == ___st45config ) return NULL ;

    ___st45config -> buf[3][2] &= (~0b110) ;
    __u01 = 0b110 & ((___st45config -> pdoAmount)<<1) ;
    ___st45config -> buf[3][2] |= __u01;

    //PDO1v - fixed at 5V and is unable to change, ignore
    //PDO2v : Load voltage (10-bit)
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    // -bit 0:1 - sector 4, byte 0, bits 6:7
    __u01 = ___st45config -> pdo[1] . Vu ;
    __u02 = __u01 & 0b11 ;
    __u02 <<= 6 ;
    ___st45config -> buf[4][0] &= (~(0b11 <<6)) ;
    ___st45config -> buf[4][0] |= __u02 ;
    __u02 = __u01 >> 2 ;
    ___st45config -> buf[4][1] = __u02 ;
    //PDO3v : Load voltage (10-bit)
    // -bit 8:9 - sector 4, byte 3, bits 0:1
    // -bit 0:7 - sector 4, byte 2, bits 0:7
    __u01 = ___st45config -> pdo[2] . Vu ;
    __u02 = __u01 >> 8;
    ___st45config -> buf[4][3] &= (~0b11) ;
    ___st45config -> buf[4][3] |= (0b11 & __u02) ;
    ___st45config -> buf[4][2] = 0xFFUL & __u01 ;

    // PDO1i : sector 3, byte 2, bits 4:7
    __u01 = ___st45config -> pdo[0] . Iu ;
    __u02 = __u01 << 4;
    ___st45config -> buf[3][2] &= 0x0F ;
    ___st45config -> buf[3][2] |= (0xF0 & __u02) ;
    // Pdo2i : sector 3, byte 4, bits 0:3
    ___st45config -> buf[3][4] &= 0xF0 ;
    ___st45config -> buf[3][4] |= (0x0F & (___st45config -> pdo[1] . Iu)) ;

    // lowerVpercent 1
    __u81 = 0xF0 & ___st45config -> buf[3][3] ;
    __u82 = 0x0F & ___st45config -> pdo[0] . lowerVpercent ;
    ___st45config -> buf[3][3] = __u81 | __u82 ;
    // lowerVpercent 2
    __u81 = 0x0F & ___st45config -> buf[3][4] ;
    __u82 = 0x0F & ___st45config -> pdo[1] . lowerVpercent ;
    __u82 <<= 4 ;
    ___st45config -> buf[3][4] = __u81 | __u82 ;
    // lowerVpercent 3
    __u81 = 0xF0 & ___st45config -> buf[3][6] ;
    __u82 = 0x0F & ___st45config -> pdo[2] . lowerVpercent ;
    ___st45config -> buf[3][6] = __u81 | __u82 ;

    // upperVpercent 1
    __u81 = 0x0F & ___st45config -> buf[3][3] ;
    __u82 = 0x0F & ___st45config -> pdo[0] . upperVpercent ;
    __u82 <<= 4 ;
    ___st45config -> buf[3][3] = __u81 | __u82 ;
    // upperVpercent 2
    __u81 = 0xF0 & ___st45config -> buf[3][5] ;
    __u82 = 0x0F & ___st45config -> pdo[1] . upperVpercent ;
    ___st45config -> buf[3][5] = __u81 | __u82 ;
    // upperVpercent 3
    __u81 = 0x0F & ___st45config -> buf[3][6] ;
    __u82 = 0x0F & ___st45config -> pdo[2] . upperVpercent ;
    __u82 <<= 4 ;
    ___st45config -> buf[3][6] = __u81 | __u82 ;

    return ___st45config ;
} // _st45_convert_config_to_nvm_buf
