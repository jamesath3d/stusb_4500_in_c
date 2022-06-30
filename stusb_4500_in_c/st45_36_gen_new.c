#include "st45_00_all.h"


extern const uint8_t _st45Default[] ;
//static uint8_t _st45new[5][8] ;
static ST45config _st45newConfig ;

void _st45_gen_new_configX(void) {


    /*
       uint8_t* __rt1 = &(_st45new[0][0]);
       float __f01 ;
       uint16_t __u01 ;
       uint16_t __u02 ;

    //memset( _st45new, 0, ST45_NVM_SIZE );
    memcpy( _st45new, _st45Default, ST45_NVM_SIZE );

    //PDO1v : ignore
    //PDO2v : Load voltage (10-bit)
    // -bit 0:1 - sector 4, byte 0, bits 6:7    
    // -bit 9:2 - sector 4, byte 1, bits 0:7
    __f01 = __Vconfig2 * 20 ;
    __u02 = __u01 = __f01 ;  if(1) printf("===---=== %.2f , 0x%04X : %2X-%2X , %2X \n" 
    , __f01 , __u01 , (0xFF&(__u01>>2)), (0xFF&(__u01>>2)), (0x0003 & __u01 ) );
    __u01 &= 0b011 ;
    __u01 <<= 6 ;
    _st45new[4][0] &= ~0b01100000 ;
    _st45new[4][0] |= __u01 ;
    __u02 &= (0xFF<<2) ;
    _st45new[4][1] &= ~(0x00FF << 2 ) ;
    _st45new[4][1] |= __u02 ;


    return __rt1 ;
    */
} // _st45_gen_new_configX

ST45config* _st45_gen_new_config( ST45config* ___st45config,
        uint8_t ___lowVp1, uint8_t ___upVp1, float __Iconfig1, 
        uint8_t ___lowVp2, uint8_t ___upVp2, float __Iconfig2, float __Vconfig2, 
        uint8_t ___lowVp3, uint8_t ___upVp3, float __Iconfig3, float __Vconfig3) {
    if ( NULL == ___st45config ) return NULL ;

    _st45_check_and_fill_config( &(_st45newConfig . pdo[0]) , 5,          __Iconfig1 , ___lowVp1, ___upVp1 ) ;
    _st45_check_and_fill_config( &(_st45newConfig . pdo[1]) , __Vconfig2, __Iconfig2 , ___lowVp2, ___upVp2 ) ;
    _st45_check_and_fill_config( &(_st45newConfig . pdo[2]) , __Vconfig3, __Iconfig3 , ___lowVp3, ___upVp3 ) ;

    _st45newConfig . pdoAmount = 3 ;

    return ___st45config ;
} // _st45_gen_new_config

