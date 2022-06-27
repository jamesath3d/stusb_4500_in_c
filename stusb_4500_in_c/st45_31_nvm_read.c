#include "st45_00_all.h"



char _st45_rBuf[5][8] = {};
char* _st45_read_top(void) {
    //int __rt = nvm_read( &(_st45_rBuf[0][0]), sizeof(_st45_rBuf) );
    int __rt = -1 ;
    if(__rt != 0) //Error
    {
        printf("819399111 : Error NVM read\r\n");
        return NULL ;
    }
    return &(_st45_rBuf[0][0]) ;
} // _st45_read_top(void) {


