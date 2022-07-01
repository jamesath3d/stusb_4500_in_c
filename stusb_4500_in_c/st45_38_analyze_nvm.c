#include "st45_00_all.h"

//static uint8_t ___st45config -> buf[5][8] ;
//static ST45config _st45analyze ;
ST45config* _st45_analyze_buf_to_gen_nvm_config( ST45config* ___st45config, uint8_t* ___nvmBuf ) {
    uint16_t  __u01 ;

    if ( NULL == ___st45config || NULL == ___nvmBuf ) return NULL ;
    memset( ___st45config, 0 , sizeof(ST45config) );
    memcpy( ___st45config -> buf, ___nvmBuf, ST45_NVM_SIZE );

    //PDO Number
    ___st45config -> pdoAmount = (___st45config -> buf[3][2]>>1) & 0x3UL ;

    //PDO1v - fixed at 5V and is unable to change
    ___st45config -> pdo[0] . Vu = 5 * 20 ;
    ___st45config -> pdo[0] . Vf = 5 ;
    //PDO2v : Load voltage (10-bit)
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    // -bit 0:1 - sector 4, byte 0, bits 6:7    
    __u01 = 0x00FFUL & ___st45config -> buf[4][1] ;
    __u01 <<= 2 ;
    __u01 |= (0x0003UL & (___st45config -> buf[4][0]>>6)) ;
    ___st45config -> pdo[1] . Vu = __u01 ;
    ___st45config -> pdo[1] . Vf = __u01 / 20.0 ;
    //PDO3v : Load voltage (10-bit)
    // -bit 8:9 - sector 4, byte 3, bits 0:1
    // -bit 0:7 - sector 4, byte 2, bits 0:7
    __u01 = 0x0003UL & ___st45config -> buf[4][3] ;
    __u01 <<= 8 ;
    __u01 |= 0x00FFUL & ___st45config -> buf[4][2] ;
    ___st45config -> pdo[2] . Vu = __u01 ;
    ___st45config -> pdo[2] . Vf = __u01 / 20.0 ;
    ___st45config -> pdo[2] . Vf = 0.05 * __u01 ;
    //printf( "VVVVu : %04X , %d, %f \n", __u01 , __u01 , ___st45config -> pdo[2] . Vf);

    // PDO1i : sector 3, byte 2, bits 4:7
    __u01 = ___st45config -> buf[3][2] ;
    __u01 >>=4 ; 
    __u01 &= 0x000FUL ;
    ___st45config -> pdo[0] . Iu = __u01 ;
    ___st45config -> pdo[0] . If = 0 ;
    if(__u01 != 0)       {
        if(__u01 < 11)     ___st45config -> pdo[0] . If = __u01 * 0.25 + 0.25 ;
        else               ___st45config -> pdo[0] . If = __u01 * 0.50 - 2.50 ;
    }
    // Pdo2i : sector 3, byte 4, bits 0:3
    __u01 = ___st45config -> buf[3][4] ; 
    __u01 &= 0x000FUL ;
    ___st45config -> pdo[1] . Iu = __u01 ;
    ___st45config -> pdo[1] . If = 0 ;
    if(__u01 != 0)      {
        if(__u01 < 11)      ___st45config -> pdo[1] . If = __u01 * 0.25 + 0.25 ;
        else                ___st45config -> pdo[1] . If = __u01 * 0.50 - 2.50 ;
    }
    //PDO3i : sector 3, byte 5, bits 4:7
    __u01 = ___st45config -> buf[3][5] ; 
    __u01 >>= 4 ;
    __u01 &= 0x000FUL ;
    ___st45config -> pdo[2] . Iu = __u01 ;
    ___st45config -> pdo[2] . If = 0 ;
    if(__u01 != 0)      {
        if(__u01 < 11)      ___st45config -> pdo[2] . If = __u01 * 0.25 + 0.25 ;
        else                ___st45config -> pdo[2] . If = __u01 * 0.50 - 2.50 ;
    }

    if(0) printf("3:3,4,5,6 -> %02hhx, %02hhx, %02hhx, %02hhx \n", 
            ___st45config -> buf[3][3], ___st45config -> buf[3][4], ___st45config -> buf[3][5], ___st45config -> buf[3][6] );
    //___st45config -> pdo[0] . lowerVpercent = 5 ;
    ___st45config -> pdo[0] . lowerVpercent =  (0x000FUL & ___st45config -> buf[3][3]) ; // maybe
    ___st45config -> pdo[1] . lowerVpercent = ((0x00F0UL & ___st45config -> buf[3][4]) >>4 ) ;
    ___st45config -> pdo[2] . lowerVpercent =  (0x000FUL & ___st45config -> buf[3][6]) ;

    ___st45config -> pdo[0] . upperVpercent = ( (0x00F0UL & ___st45config -> buf[3][3] ) >> 4 ) ;
    ___st45config -> pdo[1] . upperVpercent = ( (0x000FUL & ___st45config -> buf[3][5] ) ) ;
    ___st45config -> pdo[2] . upperVpercent = ( (0x00F0UL & ___st45config -> buf[3][6] ) >> 4 ) ;

    ___st45config -> flexCurrentU = ((0x000FUL & ___st45config -> buf[4][4])<<6) | ((0x00FCUL & ___st45config -> buf[4][3])>>2);
    ___st45config -> flexCurrentF = ___st45config -> flexCurrentU / 100.0 ; // valueU * 0.010A
    // return &_st45analyze ;
    return ___st45config ;
} // _st45_analyze_buf_to_gen_nvm_config

// https://github.com/sparkfun/SparkFun_STUSB4500_Arduino_Library.git

