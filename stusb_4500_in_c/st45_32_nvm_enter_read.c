#include "st45_00_all.h"



// CUST_EnterReadMode
int _st45_enter_nvm_read( STUSB4500_ST * ___st45LP ) {
    int __cnt ; 

    __cnt = 0 ; 

    // FTP_CUST_PASSWORD;  Set Password : 0x95 : 0x47
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CUST_PASSWORD_REG, 
                FTP_CUST_PASSWORD)  ) return -101;
    __cnt ++ ;

    //NVM Power-up Sequence
    //After STUSB start-up sequence, the NVM is powered off.

    //    NVM internal controller reset : 0x96 : 0x00
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0, 0x0 )  ) 
        return -102;
    __cnt ++ ;

    // FTP_CUST_PWR | FTP_CUST_RST_N;  Set PWR and RST_N bits : 0x96 : 0xC0
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0, 
                (FTP_CUST_PWR | FTP_CUST_RST_N) ) ) return -103;
    __cnt ++ ;

    return __cnt ;
} // _st45_enter_nvm_read

// CUST_ReadSector
int _st45_nvm_read_8_bytes( STUSB4500_ST * ___st45LP, char ___nvmBankIdx, char *___rBuf) {
    //unsigned char Buffer[2];

    //FTP_CUST_PWR | FTP_CUST_RST_N ;  // Set PWR and RST_N bits //
    if ( false == _i2c_reg_write_one_byte( ___st45LP, FTP_CTRL_0, 
                (FTP_CUST_PWR | FTP_CUST_RST_N) )) return -201;

    //(READ & FTP_CUST_OPCODE_MASK);
    if ( false == _i2c_reg_write_one_byte( ___st45LP, FTP_CTRL_1, 
                (READ & FTP_CUST_OPCODE_MASK))) return -202;// Set Read Sectors Opcode //

    //(___nvmBankIdx & FTP_CUST_SECT) |FTP_CUST_PWR |FTP_CUST_RST_N | FTP_CUST_REQ;
    if ( false == _i2c_reg_write_one_byte( ___st45LP, FTP_CTRL_0, ((___nvmBankIdx 
                        & FTP_CUST_SECT) |FTP_CUST_PWR |FTP_CUST_RST_N | FTP_CUST_REQ))) 
        return -203;  // Load Read Sectors Opcode //

    do 
    {
        if ( NULL == i2c_reg_read_one_byte( ___st45LP, FTP_CTRL_0 ) )
            return -204; // Wait for execution //
    }
    while(___st45LP->rBuf[0] & FTP_CUST_REQ); 
    //The FTP_CUST_REQ is cleared by NVM controller when the operation is finished.

    //if ( I2C_Read_USB_PD(Port,RW_BUFFER,&SectorData[0],8) != HAL_OK ) 
    if ( NULL == i2c_reg_read_bytes( ___st45LP, RW_BUFFER, 8 ) )
        return -205; // Sectors Data are available in RW-BUFFER @ 0x53 //
    memcpy( ___rBuf, ___st45LP->rBuf, 8 ) ;

    //Buffer[0] = 0 ; // NVM internal controller reset //
    if ( false == _i2c_reg_write_one_byte( ___st45LP, FTP_CTRL_0, 0x00) ) return -206;

    return 8;
} // _st45_nvm_read_8_bytes
