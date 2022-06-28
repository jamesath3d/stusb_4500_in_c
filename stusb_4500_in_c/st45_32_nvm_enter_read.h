
#ifndef  ST45_32_NVR_ENTER_READ_H__
#define  ST45_32_NVR_ENTER_READ_H__

#include "st45_00_all.h"

int _st45_enter_nvm_read( STUSB4500_ST * ___st45LP ) ;
int _st45_nvm_read_8_bytes( STUSB4500_ST * ___st45LP, char ___nvmBankIdx, char *___rBuf) ;

#endif

