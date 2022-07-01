#include "st45_00_all.h"


const uint8_t _st45Default[][8] = ST45default ;
bool _st45_cmp_buf_with_defult( uint8_t* ___cmpBuf , char* ___pr ) {
    return _st45_cmp_buf2( (uint8_t*)&_st45Default , ___cmpBuf , ___pr );
} // _st45_cmp_buf_with_defult

bool _st45_cmp_buf2( uint8_t* ___cmpBuf1 , uint8_t* ___cmpBuf2 , char* ___pr) {
    bool __rt1 ;
    bool __rt2 = true ;
    uint8_t __ii ;
    uint8_t __jj ;
    uint8_t __kk ;

    printf("834881811 :                         ========== NVM cmp : begin \n");

    for ( __ii = 0 ; __ii < 5 ; __ii ++ ) {
        __rt1 = true ;
        printf ( "%s cmp  : %01d : "  , ___pr , __ii );
        for ( __jj = 0 ; __jj < 8 ; __jj ++ ) {
            __kk = __ii*8 + __jj ;
            printf ( " %02hhX" , ___cmpBuf1[__kk] );
            if ( ___cmpBuf1[__kk] != ___cmpBuf2[__kk] ) {
                __rt1 = false;
                printf ( "(%02hhX)" , ___cmpBuf2[__kk] );
            }
        }
        if ( ! __rt1 ) {
        }
        if ( __rt1 ) {
            printf( " -- ok \n" ) ;
        } else {
            printf( " -- failed 01 \n" ) ;
            __rt2 = false ;
            printf ( "orig : %01d : "  , __ii );
            for ( __jj = 0 ; __jj < 8 ; __jj ++ ) {
                __kk = __ii*8 + __jj ;
                printf ( " %02hhX" , ___cmpBuf2[__kk] ) ;
            }
            printf( " -- default \n" ) ;
        }
    }

    printf("834881819 :                         ========== NVM cmp : end \n");

    return __rt2 ;
} // _st45_cmp_buf2

