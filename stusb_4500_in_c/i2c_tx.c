#include "i2c_tx.h"



// i2c_tx returns true of transaction is successful, false otherwise.
// An I2C transaction involves one of the following:
//   a. a write (where w_len > 0 and r_len == 0)
//   b. a read  (where w_len == 0 and r_len > 0)
//   c. a write followed by a read (where w_len > 0 and r_len > 0)
// Data to be written is read from w. Data to be read is stored in r.
bool i2c_tx(uint8_t addr, uint8_t *w, uint8_t w_len, uint8_t *r, uint8_t r_len) {
  if (w_len == 0 && r_len == 0) {
    return true;
  }
  if (w_len > 0) {
    TWI0.MADDR = addr << 1;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)));
    if ((TWI0.MSTATUS & TWI_ARBLOST_bm) || (TWI0.MSTATUS & TWI_BUSERR_bm)) {
      TWI0.MSTATUS |= TWI_ARBLOST_bm;
      return false;
    }
    while (1) {
      while (!(TWI0.MSTATUS & TWI_WIF_bm));
      if (TWI0.MSTATUS & TWI_RXACK_bm) {
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
        return false;
      }
      if (w_len == 0) {
        break;
      }
      TWI0.MDATA = *w;
      w_len--;
      w++;
    }
  }
  if (r_len > 0) {
    TWI0.MADDR = addr << 1 | 1;
    while (!(TWI0.MSTATUS & TWI_RIF_bm));
    if ((TWI0.MSTATUS & TWI_ARBLOST_bm) || (TWI0.MSTATUS & TWI_BUSERR_bm)) {
      TWI0.MSTATUS |= TWI_ARBLOST_bm;
      return false;
    }
    for (; r_len > 0; r_len--, r++) {
      while (!(TWI0.MSTATUS & TWI_RIF_bm));
      *r = TWI0.MDATA;
      TWI0.MCTRLB = (r_len > 1 ? 0 : TWI_ACKACT_bm) | TWI_MCMD_RECVTRANS_gc;
    }
  }
  TWI0.MCTRLB = TWI_MCMD_STOP_gc;
  return true;
} // i2c_tx

