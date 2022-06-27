// stusb4500_flash flashes the given configuration onto STUSB4500 NVRAM
// and returns true if successful, false otherwise.
bool stusb4500_flash(stusb4500_config cfg) {
  uint8_t nvm[] = {
    0x00, 0x00, 0xB0, 0xAA, 0x00, 0x45, 0x00, 0x00,
    0x10, 0x40, 0x9C, 0x1C, 0xFF, 0x01, 0x3C, 0xDF,
    0x02, 0x40, 0x0F, 0x00, 0x32, 0x00, 0xFC, 0xF1,
    // default: 0x00, 0x19, 0x56, 0xAF, 0xF5, 0x35, 0x5F, 0x00
    // changed to 15% lower voltage threshold from 20% for PDO2 & PDO3
    //                      0x1C  0x1D *******************************
    0x00, 0x19, 0x56, 0xAF, 0xA5, 0x35, 0x5A, 0x00,

  //0x20  0x21  0x22  0x23 *******************************************
    0x00, 0x4B, 0x90, 0x21, 0x43, 0x00, 0x40, 0xFB
  };
  nvm[0x1D] = (nvm[0x1D] & 0x0F) | cfg.i1 << 4;
  nvm[0x1C] = (nvm[0x1C] & 0xF0) | cfg.i2;
  nvm[0x22] = cfg.v1 & 0xFF;
  nvm[0x23] = (nvm[0x23] & 0b11111100) | (cfg.v1 >> 8);
  nvm[0x20] = (nvm[0x20] & 0b11000000) | ((cfg.v2 & 0b11) << 6);
  nvm[0x21] = cfg.v2 >> 2;
  if (cfg.req_pd) {
    nvm[0x26] |= 0b1000;
  }
  //------------------------------------- stusb4500_flash
  
  // STUSB4500 seems to pull down the SDA on reset and that messes
  // with internal state of the TWI. Therefore explicitly put the bus
  // into idle state.
  TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;

  enum { addr = 0x28 };
  // Flash
  if (!i2c_reg_write(addr, 0x95, 0x47)) return false;
  if (!i2c_reg_write(addr, 0x53, 0x00)) return false;
  if (!i2c_reg_write(addr, 0x96, 0x40)) return false;
  if (!i2c_reg_write(addr, 0x96, 0x00)) return false;
  delay(4);
  if (!i2c_reg_write(addr, 0x96, 0x40)) return false;
  if (!i2c_reg_write(addr, 0x97, 0xFA)) return false;
  if (!i2c_reg_write(addr, 0x96, 0x50)) return false;
  delay(4);
  if (!i2c_reg_write(addr, 0x97, 0x07)) return false;
  if (!i2c_reg_write(addr, 0x96, 0x50)) return false;
  delay(20);
  if (!i2c_reg_write(addr, 0x97, 0x05)) return false;
  if (!i2c_reg_write(addr, 0x96, 0x50)) return false;
  //------------------------------------- stusb4500_flash
  delay(20);
  for (uint8_t i = 0; i < 5; i++) {
    uint8_t buf[9] = {0x53};
    memcpy(buf + 1, nvm + (i * 8), 8);
    if (!i2c_tx(addr, buf, 9, NULL, 0)) return false;
    delay(4);
    if (!i2c_reg_write(addr, 0x97, 0x01)) return false;
    if (!i2c_reg_write(addr, 0x96, 0x50)) return false;
    delay(4);
    if (!i2c_reg_write(addr, 0x97, 0x06)) return false;
    if (!i2c_reg_write(addr, 0x96, 0x50 + i)) return false;
    delay(8);
  }
  uint8_t buf[3] = {0x96, 0x40, 0x00};
  if (!i2c_tx(addr, buf, 3, NULL, 0)) return false;
  if (!i2c_reg_write(addr, 0x95, 0x00)) return false;

  return true;
} stusb4500_flash
