#include "st45_00_all.h"



char _st45_rBuf[5][8] = {};
char* _st45_read_top(void) {
    int __rt = -1 ;
    __rt = _st45_nvm_read( &(_st45_rBuf[0][0]), sizeof(_st45_rBuf) );

    if ( 1 ) {
        for ( uint8_t __ii = 0 ; __ii < 5 ; __ii ++ ) {
            for ( uint8_t __jj = 0 ; __jj < 8 ; __jj ++ ) {
                if ( 0 == __jj ) {
                    printf ( "%02hhX"  , _st45_rBuf[__ii][__jj] );
                } else {
                    printf ( " %02hhx" , _st45_rBuf[__ii][__jj] );
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

int _st45_nvm_read(char * ___pSectorsOut, int __SectorsLength)
{
    char __data[5][8]; // must read all 40 bytes.

    memset(__data,0,sizeof(__data));
    
    if(__SectorsLength != ST45_NVM_SIZE) //error, input buffer too small
        return -2;
    
    /*
    if (CUST_EnterReadMode(0) != 0 ) return -1;
    if (CUST_ReadSector(0, 0, &__data[0][0]) != 0 ) return -1;
    if (CUST_ReadSector(0, 1, &__data[1][0]) != 0 ) return -1;
    if (CUST_ReadSector(0, 2, &__data[2][0]) != 0 ) return -1;
    if (CUST_ReadSector(0, 3, &__data[3][0]) != 0 ) return -1;
    if (CUST_ReadSector(0, 4, &__data[4][0]) != 0 ) return -1;
    if (CUST_ExitTestMode(0) != 0 ) return -1;
    */
    
    memcpy( ___pSectorsOut, __data , sizeof( __data ));
    
    return 0; //OK
}
