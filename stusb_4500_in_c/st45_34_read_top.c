#include "st45_00_all.h"

uint8_t* _st45_read_top( ST45i2cST * __st45LP ) {
    static uint8_t __st45_rBuf[5][8] = {};
    int __rt = -1 ;
    __rt = _st45_nvm_read( __st45LP, &(__st45_rBuf[0][0]), sizeof(__st45_rBuf) );

    if(_i2c_tx_debug) FP1("819399111 :                         ========== NVM read : begin \n");

    if(__rt < 0) //Error
    {
        ST45_dump_buf2( __st45_rBuf , "Error NVM read" );

        return NULL ;
    }

    if(_i2c_tx_debug) FP1("819399119 :                         ========== NVM read : succeed end r\n");
    return &(__st45_rBuf[0][0]) ;
} // _st45_read_top(void) {

