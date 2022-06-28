
#ifndef  ST45_31_NVM_READ_H__
#define  ST45_31_NVM_READ_H__

#include "st45_00_all.h"

bool _st45_comp_buf_with_defult( char* ___cmpBuf ) ;
char* _st45_read_top( STUSB4500_ST * __st45LP ) ;
int _st45_nvm_read( STUSB4500_ST * __st45LP, char * ___pSectorsOut, int __SectorsLength);

#endif

