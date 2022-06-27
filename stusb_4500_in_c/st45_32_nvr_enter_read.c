#include "st45_00_all.h"



// CUST_EnterReadMode
int _st45_enter_nvr_read( STUSB4500_ST * ___st45LP ) {
    int __cnt ; 

    __cnt = 0 ; 

    // FTP_CUST_PASSWORD;  Set Password : 0x95 : 0x47
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CUST_PASSWORD_REG, FTP_CUST_PASSWORD)  )return -101;
    __cnt ++ ;

    //NVM Power-up Sequence
    //After STUSB start-up sequence, the NVM is powered off.

    //    NVM internal controller reset : 0x96 : 0x00
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0, 0x0 )  ) return -102;
    __cnt ++ ;

    // FTP_CUST_PWR | FTP_CUST_RST_N;  Set PWR and RST_N bits : 0x96 : 0xC0
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0, (FTP_CUST_PWR | FTP_CUST_RST_N) ) ) return -103;
    __cnt ++ ;

    return __cnt ;
} // _st45_enter_nvr_read

int _st45_nvr_read_8_bytes( STUSB4500_ST * ___st45LP, char ___nvrBankIdx, char *___rBuf) {
    unsigned char Buffer[2];

    Buffer[0]= FTP_CUST_PWR | FTP_CUST_RST_N ;  /* Set PWR and RST_N bits */
    if ( I2C_Write_USB_PD(Port,FTP_CTRL_0,Buffer,1) != HAL_OK )return -1;

    Buffer[0]= (READ & FTP_CUST_OPCODE_MASK);
    if ( I2C_Write_USB_PD(Port,FTP_CTRL_1,Buffer,1) != HAL_OK )return -1;/* Set Read Sectors Opcode */

    Buffer[0]= (___nvrBankIdx & FTP_CUST_SECT) |FTP_CUST_PWR |FTP_CUST_RST_N | FTP_CUST_REQ;
    if ( I2C_Write_USB_PD(Port,FTP_CTRL_0,Buffer,1) != HAL_OK )return -1;  /* Load Read Sectors Opcode */

    do 
    {
        if ( I2C_Read_USB_PD(Port,FTP_CTRL_0,Buffer,1) != HAL_OK )return -1; /* Wait for execution */
    }
    while(Buffer[0] & FTP_CUST_REQ); //The FTP_CUST_REQ is cleared by NVM controller when the operation is finished.

    if ( I2C_Read_USB_PD(Port,RW_BUFFER,&SectorData[0],8) != HAL_OK )return -1; /* Sectors Data are available in RW-BUFFER @ 0x53 */

    Buffer[0] = 0 ; /* NVM internal controller reset */
    if ( I2C_Write_USB_PD(Port,FTP_CTRL_0,Buffer,1) != HAL_OK )return -1;

    return 0;
} // _st45_nvr_read_8_bytes
