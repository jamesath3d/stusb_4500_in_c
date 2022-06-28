#include "st45_00_all.h"


const char _st45Default[] = ST45default ;
bool _st45_comp_buf_with_defult( char* ___cmpBuf ) {
    bool __rt1 ;
    bool __rt2 = true ;
    uint8_t __ii ;
    uint8_t __jj ;
    uint8_t __kk ;

    printf("834881811 :                         ========== NVM cmp : begin \n");

    for ( __ii = 0 ; __ii < 5 ; __ii ++ ) {
        __rt1 = true ;
        printf ( "cmp  : %01d : "  , __ii );
        for ( __jj = 0 ; __jj < 8 ; __jj ++ ) {
            __kk = __ii*8 + __jj ;
            printf ( " %02hhX" , ___cmpBuf[__kk] );
            if ( ___cmpBuf[__kk] != _st45Default[__kk] ) {
                __rt1 = false;
                printf ( "(%02hhX)" , _st45Default[__kk] );
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
                printf ( " %02hhX" , _st45Default[__kk] ) ;
            }
            printf( " -- default \n" ) ;
        }
    }

    printf("834881819 :                         ========== NVM cmp : end \n");

    return __rt2 ;
} // _st45_comp_buf_with_defult

