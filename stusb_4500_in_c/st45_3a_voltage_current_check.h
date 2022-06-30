
#ifndef  ST45_3A_VOLTAGE_CURRENT_CHECK_H__
#define  ST45_3A_VOLTAGE_CURRENT_CHECK_H__

#include "st45_00_all.h"

void _st45_check_and_modifyV( uint8_t ___pdoIdx, float* ___V );
void _st45_check_and_modifyI( uint8_t ___pdoIdx, float* ___I );

#endif

