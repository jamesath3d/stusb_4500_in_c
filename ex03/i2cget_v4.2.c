/*
   i2cget.c - A user-space program to read an I2C register.
   Copyright (C) 2005-2012  Jean Delvare <jdelvare@suse.de>

   Based on i2cset.c:
   Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
   Mark D. Studebaker <mdsxyz123@yahoo.com>
   Copyright (C) 2004-2005  Jean Delvare

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301 USA.
   */

#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include "i2cbusses.h"
#include "util.h"
#include "../version.h"

static void help(void) __attribute__ ((noreturn));

static void help(void)
{
    fprintf(stderr,
            "Usage: i2cget [-f] [-y] [-a] I2CBUS CHIP-ADDRESS [DATA-ADDRESS [MODE]]\n"
            "  I2CBUS is an integer or an I2C bus name\n"
            "  ADDRESS is an integer (0x08 - 0x77, or 0x00 - 0x7f if -a is given)\n"
            "  MODE is one of:\n"
            "    b (read byte data, default)\n"
            "    w (read word data)\n"
            "    c (write byte/read byte)\n"
            "    Append p for SMBus PEC\n");
    exit(1);
} // help

static int check_funcs(int file, int size, int ___clientAddress , int pec)
{
    unsigned long funcs;

    /* check adapter functionality */
    if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
        fprintf(stderr, "Error: Could not get the adapter "
                "functionality matrix: %s\n", strerror(errno));
        return -1;
    }

    switch (size) {
        case I2C_SMBUS_BYTE:
            if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus receive byte");
                return -1;
            }
            if (___clientAddress  >= 0
                    && !(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
                return -1;
            }
            break;

            // ============================== ==== check_funcs
        case I2C_SMBUS_BYTE_DATA:
            if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus read byte");
                return -1;
            }
            break;

        case I2C_SMBUS_WORD_DATA:
            if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus read word");
                return -1;
            }
            break;
            // ============================== ==== check_funcs
    }

    if (pec
            && !(funcs & (I2C_FUNC_SMBUS_PEC | I2C_FUNC_I2C))) {
        fprintf(stderr, "Warning: Adapter does "
                "not seem to support PEC\n");
    }

    return 0;
} // check_funcs

static int confirm(const char *filename, int address, int size, int ___clientAddress ,
        int pec)
{
    int dont = 0;

    fprintf(stderr, "WARNING! This program can confuse your I2C "
            "bus, cause data loss and worse!\n");

    /* Don't let the user break his/her EEPROMs */
    if (address >= 0x50 && address <= 0x57 && pec) {
        fprintf(stderr, "STOP! EEPROMs are I2C devices, not "
                "SMBus devices. Using PEC\non I2C devices may "
                "result in unexpected results, such as\n"
                "trashing the contents of EEPROMs. We can't "
                "let you do that, sorry.\n");
        return 0;
    }
    // ============================== ==== confirm

    if (size == I2C_SMBUS_BYTE && ___clientAddress  >= 0 && pec) {
        fprintf(stderr, "WARNING! All I2C chips and some SMBus chips "
                "will interpret a write\nbyte command with PEC as a"
                "write byte data command, effectively writing a\n"
                "value into a register!\n");
        dont++;
    }

    fprintf(stderr, "I will read from device file %s, chip "
            "address 0x%02x, ", filename, address);
    if (___clientAddress  < 0)
        fprintf(stderr, "current data\naddress");
    else
        fprintf(stderr, "data address\n0x%02x", ___clientAddress );
    fprintf(stderr, ", using %s.\n",
            size == I2C_SMBUS_BYTE ? (___clientAddress  < 0 ?
                "read byte" : "write byte/read byte") :
            size == I2C_SMBUS_BYTE_DATA ? "read byte data" :
            "read word data");
    // ============================== ==== confirm
    if (pec)
        fprintf(stderr, "PEC checking enabled.\n");

    fprintf(stderr, "Continue? [%s] ", dont ? "y/N" : "Y/n");
    fflush(stderr);
    if (!user_ack(!dont)) {
        fprintf(stderr, "Aborting on user request.\n");
        return 0;
    }

    return 1;
} // confirm

int main(int argc, char *argv[])
{
    char *end;
    int res=0, i2cDevNO, __i2cClientAddress, size, file;
    int __regAddress ;
    char filename[20];
    int pec = 0;
    int flags = 0;
    int force = 0, yes = 0, version = 0, all_addrs = 0;

    /* handle (optional) flags first */
    while (1+flags < argc && argv[1+flags][0] == '-') {
        switch (argv[1+flags][1]) {
            case 'V': version = 1; break;
            case 'f': force = 1; break;
            case 'y': yes = 1; break;
            case 'a': all_addrs = 1; break;
            default:
                      fprintf(stderr, "Error: Unsupported option "
                              "\"%s\"!\n", argv[1+flags]);
                      help();
                      exit(1);
        }
        flags++;
    }
    // ============================== ==== main

    if (version) {
        fprintf(stderr, "i2cget version %s\n", VERSION);
        exit(0);
    }

    if (argc < flags + 3)
        help();

    i2cDevNO = lookup_i2c_bus(argv[flags+1]);
    if (i2cDevNO < 0)
        help();

    // ============================== ==== main
    __i2cClientAddress = parse_i2c_address(argv[flags+2], all_addrs);
    if (__i2cClientAddress < 0)
        help();

    if (argc > flags + 3) {
        size = I2C_SMBUS_BYTE_DATA;
        __regAddress = strtol(argv[flags+3], &end, 0);
        if (*end || __regAddress < 0 || __regAddress > 0xff) {
            fprintf(stderr, "Error: Data __i2cClientAddress invalid!\n");
            help();
        }
    } else {
        size = I2C_SMBUS_BYTE;
        __regAddress = -1;
    }

    if (argc > flags + 4) {
        switch (argv[flags+4][0]) {
            case 'b': size = I2C_SMBUS_BYTE_DATA; break;
            case 'w': size = I2C_SMBUS_WORD_DATA; break;
            case 'c': size = I2C_SMBUS_BYTE; break;
            default:
                      fprintf(stderr, "Error: Invalid mode!\n");
                      help();
        }
        pec = argv[flags+4][1] == 'p';
    }
    // ============================== ==== main

    if ( 0 ) {
        printf( " i2cDevNO   : %d 0x%x \n" , i2cDevNO     , i2cDevNO );
        printf( " filename : <%s>    \n" , filename );
        printf( " size     : %d 0x%x \n" , size     , size );
        printf( " pec      : %d 0x%x \n" , pec      , pec  );
        printf( " res      : %d 0x%x \n" , res      , res  );
        printf( " __i2cClientAddress  : %d 0x%x \n" , __i2cClientAddress  , __i2cClientAddress );
        printf( " __regAddress : %d 0x%x \n" , __regAddress , __regAddress );
    }

    file = open_i2c_dev(i2cDevNO, filename, sizeof(filename), 0);
    if (file < 0
            || check_funcs(file, size, __regAddress, pec)
            || set_slave_addr(file, __i2cClientAddress, force))
        exit(1);

    if (!yes && !confirm(filename, __i2cClientAddress, size, __regAddress, pec))
        exit(0);

    if (pec && ioctl(file, I2C_PEC, 1) < 0) {
        fprintf(stderr, "Error: Could not set PEC: %s\n",
                strerror(errno));
        close(file);
        exit(1);
    }
    // ============================== ==== main

    switch (size) {
        case I2C_SMBUS_BYTE:
            if (__regAddress >= 0) {
                res = i2c_smbus_write_byte(file, __regAddress);
                if (res < 0)
                    fprintf(stderr, "Warning - write failed\n");
            }
            res = i2c_smbus_read_byte(file);
            break;
        case I2C_SMBUS_WORD_DATA:
            res = i2c_smbus_read_word_data(file, __regAddress);
            break;
        default: /* I2C_SMBUS_BYTE_DATA */
            res = i2c_smbus_read_byte_data(file, __regAddress);
    }
    close(file);
    // ============================== ==== main

    if ( 1 ) {
        printf( " i2cDevNO   : %d 0x%x \n" , i2cDevNO     , i2cDevNO );
        printf( " filename : <%s>    \n" , filename );
        printf( " size     : %d 0x%x \n" , size     , size );
        printf( " pec      : %d 0x%x \n" , pec      , pec  );
        printf( " res      : %d 0x%x \n" , res      , res  );
        printf( " __i2cClientAddress  : %d 0x%x \n" , __i2cClientAddress  , __i2cClientAddress );
        printf( " __regAddress : %d 0x%x \n" , __regAddress , __regAddress );
    }


    if (res < 0) {
        fprintf(stderr, "Error: Read failed\n");
        exit(2);
    }

    printf("0x%0*x\n", size == I2C_SMBUS_WORD_DATA ? 4 : 2, res);

    exit(0);
} // main
