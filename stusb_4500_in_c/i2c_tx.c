#include "i2c_tx.h"

#define LinuxI2C 
#define LinuxI2C_device "/dev/i2c-8"
#define LinuxI2C_i2cAddr 0x28

// i2c_tx returns true of transaction is successful, false otherwise.
// An I2C transaction involves one of the following:
//   a. a write (where w_len > 0 and r_len == 0)
//   b. a read  (where w_len == 0 and r_len > 0)
//   c. a write followed by a read (where w_len > 0 and r_len > 0)
// Data to be written is read from w. Data to be read is stored in r.
#ifdef LinuxI2C
bool i2c_tx2(uint8_t addr, uint8_t *w, uint8_t w_len, uint8_t *r, uint8_t r_len) {
    return true;
} // i2c_tx2
bool i2c_tx(uint8_t addr, uint8_t *w, uint8_t w_len, uint8_t *r, uint8_t r_len) {
    int file_i2c;
    //int length;
    //unsigned char buffer[60] = {0};


    if (w_len == 0 && r_len == 0) { // no read, no write, so nothing need to be done.
        return true;
    }

    //----- OPEN THE I2C BUS -----
    char *filename = (char*) LinuxI2C_device ;
    if ((file_i2c = open(filename, O_RDWR)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf("Failed to open the i2c bus <%s>\n", filename);
        return false;
    }

    int i2c_device_addr = LinuxI2C_i2cAddr ;        //<<<<<The I2C address of the slave
    if (ioctl(file_i2c, I2C_SLAVE, i2c_device_addr) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave at i2c address <0x%x>.\n", i2c_device_addr );
        //ERROR HANDLING; you can check errno to see what went wrong
        return false;
    }

    printf("Succeed open <%s> bus at i2c address <0x%x>, and get file handle <0x%x>.\n", filename, i2c_device_addr , file_i2c);

    bool __rt1 = i2c_tx2(addr, w, w_len, r, r_len) ;

    bool __rt2 = (0 == close( file_i2c ))?true:false;

    printf(" Function %s return <%d> <%d>\n", __func__ , __rt1, __rt2 );

    return __rt1 && __rt2;
    //return true;
} // i2c_tx

#else

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

#endif
