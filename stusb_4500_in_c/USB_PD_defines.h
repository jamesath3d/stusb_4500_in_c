#define ST45_NVM_SIZE   40 // 5 * 8 == 40


// read from my STUSB4500's origin NVR. by dyn @ 2022 0627
#define ST45default  { \
    {0x00, 0x00, 0xB0, 0xAA, 0x00, 0x45, 0x00, 0x00}, \
    {0x10, 0x40, 0x9C, 0x1C, 0xFF, 0x01, 0x3C, 0xDF}, \
    {0x02, 0x40, 0x0F, 0x00, 0x32, 0x00, 0xFC, 0xF1}, \
    \
    {0x00, 0x19, 0x56, 0xAF, 0xF5, 0x35, 0x5F, 0x00}, \
    {0x00, 0x4B, 0x90, 0x21, 0x43, 0x00, 0x40, 0xFB} }

/*
 * i2cdump -y 8 40
 No size specified (using byte-data access)
 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 00 00 00 00 00 00 12 00 11 20 00 63 fb 01 41 0a    ......?.? .c??A?
10: 0a 1c 00 40 00 82 0c 2c 00 00 00 21 01 80 00 00    ??.@.??,...!??..
20: 10 c8 5f 00 00 9c 00 02 02 18 00 32 05 00 00 25    ??_..?.???.2?..%
30: 00 66 0b 56 07 ac e5 20 d1 02 00 2c c1 03 00 2c    .f?V??? ??.,??.,
40: b1 04 00 f4 41 06 00 d5 87 fa 96 00 00 18 e3 00    ??.?A?.????..??.
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
70: 43 c8 90 01 08 64 d0 02 00 2c b1 04 00 f4 b1 04    C????d??.,??.???
80: 00 5e fd 01 00 96 90 01 10 96 b0 04 00 64 40 06    .^??.???????.d@?
90: 00 f4 91 01 50 00 40 00 00 00 00 00 00 00 00 00    .???P.@.........
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
*/

/*
   PDO amount is : 3 , FlexCurrent is 0xC8-2.000000
   PDO 1 V,I is : 0x064- 5.00v ,  0x5-1.50A , lowVp: 15%, upVp: 10%
   PDO 2 V,I is : 0x12C-15.00v ,  0x5-1.50A , lowVp: 15%, upVp: 5%
   PDO 3 V,I is : 0x190-20.00v ,  0x3-1.00A , lowVp: 15%, upVp: 5%
   */

/*Identification of STUSB */
#define DEVICE_ID 0x2F
#define EXTENTION_10  (uint8_t )0x80
#define ID_Reg (uint8_t )0x1C 
#define CUT (uint8_t )3<<2
#define CUT_A (uint8_t )4 << 2 
#define CUT_gen1S (uint8_t )8 << 2 

#define DEV_CUT (uint8_t )0x03

/*NVM FLasher Registers Definition */

#define FTP_CUST_PASSWORD_REG	0x95
#define FTP_CUST_PASSWORD		0x47
#define FTP_CTRL_0              0x96
#define FTP_CUST_PWR	0x80 
#define FTP_CUST_RST_N	0x40
#define FTP_CUST_REQ	0x10
#define FTP_CUST_SECT 0x07
#define FTP_CTRL_1              0x97
#define FTP_CUST_SER_MASK 0xF8
#define FTP_CUST_OPCODE_MASK 0x07
#define RW_BUFFER 0x53


/* FTP_CUST_OPCODE field values */
#define READ            0x00 //Read memory array
#define WRITE_PL        0x01 //Shift In Data on Program Load (PL) Register
#define WRITE_SER       0x02 //Shift In Data on Sector Erase (SER) Register 
#define READ_PL	        0x03 //Shift Out Data on Program Load (PL) Register
#define READ_SER        0x04 //Shift Out Data on sector Erase (SER) Register
#define ERASE_SECTOR    0x05 //Erase memory array
#define PROG_SECTOR     0x06 //Program 256b word into EEPROM
#define SOFT_PROG_SECTOR 0x07 //Soft Program array

/* FTP_CUST_SER field values */
#define	SECTOR_0	0x01
#define	SECTOR_1	0x02
#define	SECTOR_2	0x04
#define	SECTOR_3	0x08
#define	SECTOR_4	0x10








