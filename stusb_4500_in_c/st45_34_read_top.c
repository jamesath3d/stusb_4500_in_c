#include "st45_00_all.h"

char* _st45_read_top( STUSB4500_ST * __st45LP ) {
    static char __st45_rBuf[5][8] = {};
    int __rt = -1 ;
    __rt = _st45_nvm_read( __st45LP, &(__st45_rBuf[0][0]), sizeof(__st45_rBuf) );

    printf("819399111 :                         ========== NVM read : begin \n");
    if ( 1 ) {
        for ( uint8_t __ii = 0 ; __ii < 5 ; __ii ++ ) {
            for ( uint8_t __jj = 0 ; __jj < 8 ; __jj ++ ) {
                if ( 0 == __jj ) {
                    printf ( "%02hhX"  , __st45_rBuf[__ii][__jj] );
                } else {
                    printf ( " %02hhX" , __st45_rBuf[__ii][__jj] );
                }
            }
            if ( 2 == __ii ) {
                //printf( "\n\n" ) ;
                printf( "\n" ) ;
            } else {
                printf( "\n" ) ;
            }
        }

    }

    if(__rt < 0) //Error
    {
        printf("819399113 : Error NVM read\n");
        return NULL ;
    }

    printf("819399119 :                         ========== NVM read : succeed end r\n");
    return &(__st45_rBuf[0][0]) ;
} // _st45_read_top(void) {

