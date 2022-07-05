#include "st45_00_all.h"


// CUST_EnterWriteMode
// CUST_EnterReadMode
// _st45_enter_nvm_read
int _st45_enter_nvm_write( ST45i2cST * ___st45LP , unsigned char ___erasedSector ) {
    int __cnt ;
    uint8_t __u01 ;

    __cnt = 0 ;

    __u01 = FTP_CUST_PASSWORD; // FTP_CUST_PASSWORD;  Set Password : 0x95 w: 0x47
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CUST_PASSWORD_REG,
                __u01 )  ) EXi( -201 ) ;
    __cnt ++ ; // 1


    
    // this register must be NULL for Partial Erase feature : 0x53 w: 0x00
    __u01 = 0x00 ; 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , RW_BUFFER,
                __u01)  ) EXi( -211 ) ;
    __cnt ++ ; // 2

    
    {  
        //NVM Power-up Sequence
        //After STUSB start-up sequence, the NVM is powered off.
        
        __u01 = 0x00 ; // NVM internal controller reset // 0x96 w: 0x00
        if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                    __u01)  ) EXi( -213 ) ;
        __cnt ++ ; // 3


        __u01 = FTP_CUST_PWR | FTP_CUST_RST_N; // Set PWR and RST_N bits 
        if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                    __u01) ) EXi( -214 ) ;
        __cnt ++ ; // 4 // 0x96 w: 0xC0
    }


    __u01 =((___erasedSector << 3) & FTP_CUST_SER_MASK) | ( WRITE_SER & FTP_CUST_OPCODE_MASK) ;  
    // Load 0xF1 to erase all sectors of FTP and Write SER Opcode 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_1,
                __u01) ) EXi( -215 ) ; // Set Write SER Opcode 
    __cnt ++ ; // 5 // 0x97 w:0xFA

    __u01 =FTP_CUST_PWR | FTP_CUST_RST_N | FTP_CUST_REQ ; 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                __u01) ) EXi( -217 ) ; // Load Write SER Opcode 
    __cnt ++ ; // 6 // 0x96 w: 0xD0

    do 
    {
        // Wait for execution 1 // read from 0x96
        if ( NULL == _i2c_reg_read_one_byte( ___st45LP , FTP_CTRL_0 ) ) EXi( -218 );
    }
    while(___st45LP->rBuf[0]  & FTP_CUST_REQ);  // 1st read got : 0xC0, mask 0x10, exit loop

    __u01 =  SOFT_PROG_SECTOR & FTP_CUST_OPCODE_MASK ;  
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_1,
                __u01) ) EXi( -221 ) ; // Set Soft Prog Opcode 
    __cnt ++ ; // 7 // 0x97 w: 0x07

    __u01 =FTP_CUST_PWR | FTP_CUST_RST_N | FTP_CUST_REQ ; 
    // Load Soft Prog Opcode 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                __u01) ) EXi( -221 ) ; 
    __cnt ++ ; // Set Soft Prog Opcode // 0x96 w: 0xD0 // 8


    do 
    {
        // Wait for execution 2 // read from 0x96
        if ( NULL == _i2c_reg_read_one_byte( ___st45LP , FTP_CTRL_0 ) ) EXi( -223 );
    }
    while(___st45LP->rBuf[0]  & FTP_CUST_REQ); // read 0xD0 , failed ; read 0xC0, ok, exit loop

    __u01 = ERASE_SECTOR & FTP_CUST_OPCODE_MASK ;  
    // Set Erase Sectors Opcode 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_1,
                __u01) ) EXi( -231 ) ; 
    __cnt ++ ; // Set Soft Prog Opcode  // 9 // 0x97 w:0x05


    __u01 =FTP_CUST_PWR | FTP_CUST_RST_N | FTP_CUST_REQ ;  
    // Load Erase Sectors Opcode 
    if ( false == _i2c_reg_write_one_byte( ___st45LP , FTP_CTRL_0,
                __u01) ) EXi( -232 ) ; 
    __cnt ++ ; // Set Soft Prog Opcode  // 10 // 0x96 w:0xD0


    do 
    {
        // Wait for execution 3 // // read from 0x96
        if ( NULL == _i2c_reg_read_one_byte( ___st45LP , FTP_CTRL_0 ) ) EXi( -234 );
    }
    while(___st45LP->rBuf[0]  & FTP_CUST_REQ);	// read 0xD0,0xD0,0xD0, failed ; 0xC0, ok-exit loop

    if(0) FP( "now __cnt is %d \n" , __cnt ) ;
    return __cnt;

} // _st45_enter_nvm_write
