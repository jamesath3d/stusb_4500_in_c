#include "st45_00_all.h"

int _st45_nvm_read( STUSB4500_ST * __st45LP, uint8_t * ___pSectorsOut, int __SectorsLength) {
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
