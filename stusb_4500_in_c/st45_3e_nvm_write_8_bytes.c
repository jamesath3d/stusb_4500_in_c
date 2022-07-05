#include "st45_00_all.h"


// CUST_WriteSector
// _st45_nvm_read_8_bytes
int _st45_nvm_write_8_bytes( ST45i2cST * ___st45LP, uint8_t ___nvmBankIdx, uint8_t *___wBuf) {
    //unsigned char Buffer[2];
    uint8_t __u01 ; 
    int __cnt ;

    if ( NULL == ___st45LP || NULL == ___wBuf ) EXi( -301 );
    
    __cnt = 0 ;

    //Write the 64-bit data to be written in the sector
    if ( false == _i2c_reg_write_bytes( ___st45LP , RW_BUFFER,
                ___wBuf , 8 )  ) EXi( -303 ) ;
    __cnt += 8 ; // 8

    
    __u01=FTP_CUST_PWR | FTP_CUST_RST_N; /*Set PWR and RST_N bits*/
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                    __u01)  ) EXi( -311 ) ;
    __cnt ++ ; // 9

    
    //NVM Program Load Register to write with the 64-bit data to be written in sector
    __u01= (WRITE_PL & FTP_CUST_OPCODE_MASK); /*Set Write to PL Opcode*/
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_1,
                __u01) ) EXi( -313 ) ; 
    __cnt ++ ; // 10 // 

    
    __u01=FTP_CUST_PWR |FTP_CUST_RST_N | FTP_CUST_REQ;  /* Load Write to PL Sectors Opcode */  
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                    __u01)  ) EXi( -315 ) ;
    __cnt ++ ; // 11
    
    do 
    {
        // Wait for execution 1 // read from 0x96
        if ( NULL == _i2c_reg_read_one_byte( ___st45LP , FTP_CTRL_0 ) ) EXi( -218 );
    }		 
    while(___st45LP->rBuf[0] & FTP_CUST_REQ) ; //FTP_CUST_REQ clear by NVM controller
    
    
    //NVM "Word Program" operation to write the Program Load Register in the sector to be written
    __u01= (PROG_SECTOR & FTP_CUST_OPCODE_MASK); /*Set Prog Sectors Opcode*/
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_1,
                __u01) ) EXi( -317 ) ;
    __cnt ++ ; // 12 //
    
    /* Load Prog Sectors Opcode */  
    __u01=(___nvmBankIdx & FTP_CUST_SECT) |FTP_CUST_PWR |FTP_CUST_RST_N | FTP_CUST_REQ;
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                    __u01)  ) EXi( -321 ) ;
    __cnt ++ ; // 13

    
    do 
    {
        // Wait for execution 1 // read from 0x96
        if ( NULL == _i2c_reg_read_one_byte( ___st45LP , FTP_CTRL_0 ) ) EXi( -218 );
    }
    while(___st45LP->rBuf[0] & FTP_CUST_REQ); //FTP_CUST_REQ clear by NVM controller

    if(0) FP(" return : %d\n" , __cnt ) ; 
    return __cnt  ;
} // _st45_nvm_write_8_bytes
