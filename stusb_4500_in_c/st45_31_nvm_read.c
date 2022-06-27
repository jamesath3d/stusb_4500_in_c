#include "st45_00_all.h"



char _st45_rBuf[5][8] = {};
char* _st45_read_top(void) {
    //int __rt = nvm_read( &(_st45_rBuf[0][0]), sizeof(_st45_rBuf) );
    int __rt = -1 ;

    if ( 1 ) {
        for ( uint8_t __ii = 0 ; __ii < 5 ; __ii ++ ) {
            for ( uint8_t __jj = 0 ; __jj < 8 ; __jj ++ ) {
                if ( 0 == __jj ) {
                    printf ( "%02x"  , _st45_rBuf[__ii][__jj] );
                } else {
                    printf ( " %02x" , _st45_rBuf[__ii][__jj] );
                }
            }
            if ( 2 == __ii ) {
                printf( "\n\n" ) ;
            } else {
                printf( "\n" ) ;
            }
        }

    }

    if(__rt < 0) //Error
    {
        printf("819399111 : Error NVM read\r\n");
        return NULL ;
    }

    return &(_st45_rBuf[0][0]) ;
} // _st45_read_top(void) {


