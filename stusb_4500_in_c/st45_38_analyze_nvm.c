#include "st45_00_all.h"

static char _st45nvm[5][8] ;
static ST45config _st45analyze ;
ST45config* _st45_analyze_nvm( char* ___nvmBuf ) {
    uint16_t  __u01 ;

    memcpy( _st45nvm, ___nvmBuf, ST45_NVM_SIZE );

    //PDO Number
    _st45analyze . pdoAmount = (_st45nvm[3][2]>>1) & 0x3 ;

    //PDO1v - fixed at 5V and is unable to change
    _st45analyze . pdo1 . Vu = 5 * 20 ;
    _st45analyze . pdo1 . Vf = 5 ;
    //PDO2v : Load voltage (10-bit)
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    // -bit 0:1 - sector 4, byte 0, bits 6:7    
    __u01 = 0x00FF & _st45nvm[4][1] ;
    __u01 <<= 2 ;
    __u01 |= (0x0003 & (_st45nvm[4][0]>>6)) ;
    _st45analyze . pdo2 . Vu = __u01 ;
    _st45analyze . pdo2 . Vf = __u01 / 20.0 ;
    //PDO3v : Load voltage (10-bit)
    // -bit 8:9 - sector 4, byte 3, bits 0:1
    // -bit 0:7 - sector 4, byte 2, bits 0:7
    __u01 = 0x0003 & _st45nvm[4][3] ;
    __u01 <<= 8 ;
    __u01 |= 0x00FF & _st45nvm[4][2] ;
    _st45analyze . pdo3 . Vu = __u01 ;
    _st45analyze . pdo3 . Vf = __u01 / 20.0 ;

    // PDO1i : sector 3, byte 2, bits 4:7
    __u01 = _st45nvm[3][2] ;
    __u01 >>=4 ; 
    __u01 &= 0x000F ;
    _st45analyze . pdo1 . Iu = __u01 ;
    _st45analyze . pdo1 . If = 0 ;
    if(__u01 != 0)       {
        if(__u01 < 11)     _st45analyze . pdo1 . If = __u01 * 0.25 + 0.25 ;
        else               _st45analyze . pdo1 . If = __u01 * 0.50 - 2.50 ;
    }

    // Pdo2i : sector 3, byte 4, bits 0:3
    __u01 = _st45nvm[3][4] ; 
    __u01 &= 0x000F ;
    _st45analyze . pdo2 . Iu = __u01 ;
    _st45analyze . pdo2 . If = 0 ;
    if(__u01 != 0)      {
        if(__u01 < 11)      _st45analyze . pdo2 . If = __u01 * 0.25 + 0.25 ;
        else                _st45analyze . pdo2 . If = __u01 * 0.50 - 2.50 ;
    }

    //PDO3i : sector 3, byte 5, bits 4:7
    __u01 = _st45nvm[3][5] ; 
    __u01 >>= 4 ;
    __u01 &= 0x000F ;
    _st45analyze . pdo3 . Iu = __u01 ;
    _st45analyze . pdo3 . If = 0 ;

    if(__u01 != 0)      {
        if(__u01 < 11)      _st45analyze . pdo3 . If = __u01 * 0.25 + 0.25 ;
        else                _st45analyze . pdo3 . If = __u01 * 0.50 - 2.50 ;
    }


    return &_st45analyze ;
} // _st45_analyze_nvm

void _st45_dump_st45pdo( ST45config* ___nvmConfig , char ___pdoIdx ) {
    ST45pdo* __pdo ;
    __pdo = &(___nvmConfig -> pdo1) ;
    __pdo += ___pdoIdx - 1;

    printf( " PDO %d V,I is : 0x%03X-%5.2fv ,  0x%1X-%4.2fA \n"
            , ___pdoIdx
            , __pdo -> Vu , __pdo -> Vf
            , __pdo -> Iu , __pdo -> If
          );

} // _st45_dump_st45pdo

void _st45_dump_st45config( ST45config* ___nvmConfig ) {
    printf( " PDO amount is : %d \n", ___nvmConfig -> pdoAmount );
    _st45_dump_st45pdo( ___nvmConfig, 1 );
    _st45_dump_st45pdo( ___nvmConfig, 2 );
    _st45_dump_st45pdo( ___nvmConfig, 3 );
} // _st45_dump_st45config
