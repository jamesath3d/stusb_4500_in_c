#include "st45_00_all.h"


//extern const uint8_t _st45Default[] ;
//static uint8_t _st45new[5][8] ;
//static ST45config _st45newConfig ;

ST45config* _st45_gen_new_config( ST45config* ___st45config,
        uint8_t ___lowVp1, uint8_t ___upVp1, float __Iconfig1, 
        uint8_t ___lowVp2, uint8_t ___upVp2, float __Iconfig2, float __Vconfig2, 
        uint8_t ___lowVp3, uint8_t ___upVp3, float __Iconfig3, float __Vconfig3) {
    if ( NULL == ___st45config ) return NULL ;

    _st45_check_and_fill_config( &(___st45config -> pdo[0]) , 5,          __Iconfig1 , ___lowVp1, ___upVp1 ) ;
    _st45_check_and_fill_config( &(___st45config -> pdo[1]) , __Vconfig2, __Iconfig2 , ___lowVp2, ___upVp2 ) ;
    _st45_check_and_fill_config( &(___st45config -> pdo[2]) , __Vconfig3, __Iconfig3 , ___lowVp3, ___upVp3 ) ;

    ___st45config -> pdoAmount = 3 ;

    return ___st45config ;
} // _st45_gen_new_config

