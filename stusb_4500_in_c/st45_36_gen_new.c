#include "st45_00_all.h"


extern const uint8_t _st45Default[] ;
static uint8_t _st45new[5][8] ;
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

uint8_t* _st45_gen_new_config( float __Iconfig1, 
        float __Iconfig2, float __Vconfig2, 
        float __Iconfig3, float __Vconfig3) {

    _st45_check_and_modifyV( 2, &__Vconfig2 ) ;
    _st45_check_and_modifyV( 3, &__Vconfig3 ) ;
    _st45_check_and_modifyI( 1, &__Iconfig1 ) ;
    _st45_check_and_modifyI( 2, &__Iconfig2 ) ;
    _st45_check_and_modifyI( 3, &__Iconfig3 ) ;

    _st45newConfig . pdoAmount = 3 ;

    _st45newConfig . pdo[0] . Vf = 5 ;
    _st45newConfig . pdo[0] . Vu = 5 * 20 ;
    _st45newConfig . pdo[1] . Vf = __Vconfig2 ;
    _st45newConfig . pdo[1] . Vu = __Vconfig2 * 20 ;
    _st45newConfig . pdo[2] . Vf = __Vconfig3 ;
    _st45newConfig . pdo[2] . Vu = __Vconfig3 * 20 ;

    _st45newConfig . pdo[0] . If = __Iconfig1 ;
    _st45newConfig . pdo[0] . Iu = __Iconfig1 * 20 ;
    _st45newConfig . pdo[1] . If = __Iconfig2 ;
    _st45newConfig . pdo[1] . Iu = __Iconfig2 * 20 ;
    _st45newConfig . pdo[2] . If = __Iconfig3 ;
    _st45newConfig . pdo[2] . Iu = __Iconfig3 * 20 ;

    return &(_st45new[0][0]) ;
} // _st45_gen_new_config

