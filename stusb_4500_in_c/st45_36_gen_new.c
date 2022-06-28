#include "st45_00_all.h"


extern const char _st45Default[] ;
static char _st45new[ST45_NVM_SIZE] ;
char* _st45_gen_new_config( uint16_t __Iconfig1, 
        uint16_t __Iconfig2, uint16_t __Vconfig2, 
        uint16_t __Iconfig3, uint16_t __Vconfig3) {
    char* __rt1 = NULL;

    memcpy( _st45new, _st45Default, ST45_NVM_SIZE );


    return __rt1 ;
} // _st45_gen_new_config

