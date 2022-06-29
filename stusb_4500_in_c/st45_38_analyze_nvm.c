#include "st45_00_all.h"

static char _st45nvm[5][8] ;
static ST45config _st45analyze ;
ST45config* _st45_analyze_nvm( char* ___nvmBuf ) {
    //uint8_t  __u01 ;

    memcpy( _st45nvm, ___nvmBuf, ST45_NVM_SIZE );

    // NVM settings get loaded into the volatile registers after a hard reset or power cycle.
    // Below we will copy over some of the saved NVM settings to the I2C registers
    //uint8_t currentValue;

    //PDO Number
    //setPdoNumber((_st45nvm[3][2] & 0x06)>>1);
    _st45analyze . pdoAmount = (_st45nvm[3][2]>>1) & 0x3 ;



    /*
    //PDO1 - fixed at 5V and is unable to change
    setVoltage(1,5.0);

    currentValue = (_st45nvm[3][2]&0xF0) >> 4;
    if(currentValue == 0)      setCurrent(1,0);
    else if(currentValue < 11) setCurrent(1,currentValue * 0.25 + 0.25);
    else                       setCurrent(1,currentValue * 0.50 - 2.50);

    //PDO2
    setVoltage(2,((_st45nvm[4][1]<<2) + (_st45nvm[4][0]>>6))/20.0);

    currentValue = (_st45nvm[3][4]&0x0F);
    if(currentValue == 0)      setCurrent(2,0);
    else if(currentValue < 11) setCurrent(2,currentValue * 0.25 + 0.25);
    else                       setCurrent(2,currentValue * 0.50 - 2.50);

    //PDO3
    setVoltage(3,(((_st45nvm[4][3]&0x03)<<8) + _st45nvm[4][2])/20.0);

    currentValue = (_st45nvm[3][5]&0xF0) >> 4;
    if(currentValue == 0)      setCurrent(3,0);
    else if(currentValue < 11) setCurrent(3,currentValue * 0.25 + 0.25);
    else                       setCurrent(3,currentValue * 0.50 - 2.50);
    */

    return &_st45analyze ;
} // _st45_analyze_nvm

void _st45_dump_st45config( ST45config* ___nvmBuf ) {
    printf( " PDO amount is : %d \n", ___nvmBuf -> pdoAmount );
} // _st45_dump_st45config
