#include "st45_00_all.h"

void _st45_analyze_nvm( char* ___nvmBuf ) {


  // NVM settings get loaded into the volatile registers after a hard reset or power cycle.
  // Below we will copy over some of the saved NVM settings to the I2C registers
  //uint8_t currentValue;

  //PDO Number
  setPdoNumber((sector[3][2] & 0x06)>>1);


  /*
  //PDO1 - fixed at 5V and is unable to change
  setVoltage(1,5.0);

  currentValue = (sector[3][2]&0xF0) >> 4;
  if(currentValue == 0)      setCurrent(1,0);
  else if(currentValue < 11) setCurrent(1,currentValue * 0.25 + 0.25);
  else                       setCurrent(1,currentValue * 0.50 - 2.50);

  //PDO2
  setVoltage(2,((sector[4][1]<<2) + (sector[4][0]>>6))/20.0);

  currentValue = (sector[3][4]&0x0F);
  if(currentValue == 0)      setCurrent(2,0);
  else if(currentValue < 11) setCurrent(2,currentValue * 0.25 + 0.25);
  else                       setCurrent(2,currentValue * 0.50 - 2.50);

  //PDO3
  setVoltage(3,(((sector[4][3]&0x03)<<8) + sector[4][2])/20.0);

  currentValue = (sector[3][5]&0xF0) >> 4;
  if(currentValue == 0)      setCurrent(3,0);
  else if(currentValue < 11) setCurrent(3,currentValue * 0.25 + 0.25);
  else                       setCurrent(3,currentValue * 0.50 - 2.50);
  */

} // _st45_analyze_nvm

