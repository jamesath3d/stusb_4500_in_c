#include "st45_00_all.h"

static uint8_t _st45nvm[5][8] ;
static ST45config _st45analyze ;
ST45config* _st45_analyze_nvm( uint8_t* ___nvmBuf ) {
    uint16_t  __u01 ;

    memcpy( _st45nvm, ___nvmBuf, ST45_NVM_SIZE );

    //PDO Number
    _st45analyze . pdoAmount = (_st45nvm[3][2]>>1) & 0x3 ;

    //PDO1v - fixed at 5V and is unable to change
    _st45analyze . pdo[0] . Vu = 5 * 20 ;
    _st45analyze . pdo[0] . Vf = 5 ;
    //PDO2v : Load voltage (10-bit)
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    // -bit 0:1 - sector 4, byte 0, bits 6:7    
    __u01 = 0x00FF & _st45nvm[4][1] ;
    __u01 <<= 2 ;
    __u01 |= (0x0003 & (_st45nvm[4][0]>>6)) ;
    _st45analyze . pdo[1] . Vu = __u01 ;
    _st45analyze . pdo[1] . Vf = __u01 / 20.0 ;
    //PDO3v : Load voltage (10-bit)
    // -bit 8:9 - sector 4, byte 3, bits 0:1
    // -bit 0:7 - sector 4, byte 2, bits 0:7
    __u01 = 0x0003 & _st45nvm[4][3] ;
    __u01 <<= 8 ;
    __u01 |= 0x00FF & _st45nvm[4][2] ;
    _st45analyze . pdo[2] . Vu = __u01 ;
    _st45analyze . pdo[2] . Vf = __u01 / 20.0 ;

    // PDO1i : sector 3, byte 2, bits 4:7
    __u01 = _st45nvm[3][2] ;
    __u01 >>=4 ; 
    __u01 &= 0x000F ;
    _st45analyze . pdo[0] . Iu = __u01 ;
    _st45analyze . pdo[0] . If = 0 ;
    if(__u01 != 0)       {
        if(__u01 < 11)     _st45analyze . pdo[0] . If = __u01 * 0.25 + 0.25 ;
        else               _st45analyze . pdo[0] . If = __u01 * 0.50 - 2.50 ;
    }
    // Pdo2i : sector 3, byte 4, bits 0:3
    __u01 = _st45nvm[3][4] ; 
    __u01 &= 0x000F ;
    _st45analyze . pdo[1] . Iu = __u01 ;
    _st45analyze . pdo[1] . If = 0 ;
    if(__u01 != 0)      {
        if(__u01 < 11)      _st45analyze . pdo[1] . If = __u01 * 0.25 + 0.25 ;
        else                _st45analyze . pdo[1] . If = __u01 * 0.50 - 2.50 ;
    }
    //PDO3i : sector 3, byte 5, bits 4:7
    __u01 = _st45nvm[3][5] ; 
    __u01 >>= 4 ;
    __u01 &= 0x000F ;
    _st45analyze . pdo[2] . Iu = __u01 ;
    _st45analyze . pdo[2] . If = 0 ;

    if(__u01 != 0)      {
        if(__u01 < 11)      _st45analyze . pdo[2] . If = __u01 * 0.25 + 0.25 ;
        else                _st45analyze . pdo[2] . If = __u01 * 0.50 - 2.50 ;
    }

    _st45analyze . pdo[0] . lowerV = 0 ;
    _st45analyze . pdo[1] . lowerV = 5 + ((0x000F & _st45nvm[3][4]) >>4 ) ;
    _st45analyze . pdo[2] . lowerV = 5 +  (0x000F & _st45nvm[3][6]) ;

    _st45analyze . pdo[0] . upperV = 5 + ( (0x000F & _st45nvm[3][3] ) >> 4 ) ;
    _st45analyze . pdo[1] . upperV = 5 + ( (0x000F & _st45nvm[3][5] ) ) ;
    _st45analyze . pdo[2] . upperV = 5 + ( (0x000F & _st45nvm[3][6] ) >> 4 ) ;

    _st45analyze . flexCurrentU = ((0x000F & _st45nvm[4][4])<<6) | ((0x00FC & _st45nvm[4][3])>>2);
    _st45analyze . flexCurrentF = _st45analyze . flexCurrentU / 100.0 ;
    return &_st45analyze ;
} // _st45_analyze_nvm

// https://github.com/sparkfun/SparkFun_STUSB4500_Arduino_Library.git

