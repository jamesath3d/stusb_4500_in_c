#include "st45_00_all.h"



// CUST_EnterReadMode
int _st45_enter_nvr_read( STUSB4500_ST * __st45LP ) {
    unsigned char Buffer[2];
    
    //Buffer[0]=FTP_CUST_PASSWORD;  /* Set Password*/
    if ( false == _i2c_reg_write_one_byte( __st45LP , FTP_CUST_PASSWORD_REG, FTP_CUST_PASSWORD)  )return -1;
    
    {  
        //NVM Power-up Sequence
        //After STUSB start-up sequence, the NVM is powered off.
        
        //Buffer[0]= 0;  /* NVM internal controller reset */
        if ( false == _i2c_reg_write_one_byte( __st45LP , FTP_CTRL_0, 0x0 )  ) return -1;
        
        //Buffer[0]= FTP_CUST_PWR | FTP_CUST_RST_N; /* Set PWR and RST_N bits */
        if ( false == _i2c_reg_write_one_byte( __st45LP , FTP_CTRL_0, (FTP_CUST_PWR | FTP_CUST_RST_N) ) ) return -1;
    }
    
    return 0 ;
} // _st45_enter_nvr_read
