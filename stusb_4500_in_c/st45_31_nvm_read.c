#include "st45_00_all.h"


const char _st45Default[] = ST45default ;
bool _st45_comp_buf_with_defult( STUSB4500_ST * __st45LP , char* ___cmpBuf ) {
    printf("834881811 :                         ========== NVM cmp : begin \n");
    if ( 1 ) {
        for ( uint8_t __ii = 0 ; __ii < 5 ; __ii ++ ) {
            printf ( "cmp : %01d : "  , __ii );
            for ( uint8_t __jj = 0 ; __jj < 8 ; __jj ++ ) {
                printf ( " %02hhX" , __st45_rBuf[__ii][__jj] );
            }
            if ( 2 == __ii ) {
                printf( "\n\n" ) ;
            } else {
                printf( "\n" ) ;
            }
        }

    }

    return true ;
} // _st45_comp_buf_with_defult

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
                printf( "\n\n" ) ;
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

int _st45_nvm_read( STUSB4500_ST * __st45LP, char * ___pSectorsOut, int __SectorsLength) {
    static char __rBuf[5][8]; // must read all 40 bytes.

    memset(__rBuf,0,sizeof(__rBuf));

    if(__SectorsLength != ST45_NVM_SIZE) //error, input buffer too small
        return -2;

    if ( 3 != _st45_enter_nvm_read( __st45LP ) ) return -1;

    if ( 8 != _st45_nvm_read_8_bytes(__st45LP, 0, &(__rBuf[0][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(__st45LP, 1, &(__rBuf[1][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(__st45LP, 2, &(__rBuf[2][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(__st45LP, 3, &(__rBuf[3][0])) ) return -1;
    if ( 8 != _st45_nvm_read_8_bytes(__st45LP, 4, &(__rBuf[4][0])) ) return -1;

    memcpy( ___pSectorsOut, __rBuf , sizeof( __rBuf ));

    return 0; //OK
} // _st45_nvm_read
