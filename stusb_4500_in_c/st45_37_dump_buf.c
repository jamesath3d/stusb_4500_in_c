#include "st45_00_all.h"

// ST45_dump_buf3 
void _st45_dump_buf( char* ___cmpBuf , char* ___pre, char* ___pre2, char* ___tail ) {
    char __preX[256] ;
    uint8_t __ii ; 
    uint8_t __jj ; 
    uint8_t __kk ; 

    if ( strlen(___pre) < 7 ) {
        strcpy( __preX, ___pre );
    } else {
        strncpy( __preX, ___pre + 6 , 8 );
        __preX[8] = 0 ;
    }
    printf("834983811 :                         ========== %s : begin \n", ___pre);
    for ( __ii = 0 ; __ii < 5 ; __ii ++ ) {
        printf ( "%s:%s  : %01d : "  , __preX, ___pre2, __ii );
        for ( __jj = 0 ; __jj < 8 ; __jj ++ ) {
            __kk = __ii*8 + __jj ;
            printf ( " %02hhX" , ___cmpBuf[__kk] );
        }
        printf( " -- %s \n" , ___tail ) ;
    }
    printf("834983819 :                         ========== %s : end \n", ___pre);
} // _st45_comp_buf_with_defult

