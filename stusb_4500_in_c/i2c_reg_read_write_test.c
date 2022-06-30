#include "i2c_reg_read_write_test.h"

inline void gap01(void){
    usleep(1000);
    printf("\n");
} // gap01


bool stusb4500_read_byte_test( ST45i2cST * __st45LP ){
    char * __ch ;

    __ch = 
        i2c_reg_read_one_byte( __st45LP, 0x06) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918381 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        i2c_reg_read_one_byte( __st45LP, 0x06) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        i2c_reg_read_one_byte( __st45LP, 0x08) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        i2c_reg_read_one_byte( __st45LP, 0x08) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        i2c_reg_read_bytes( __st45LP, 0x06, 4 ) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918385 : get from Reg <0x%0x> : <%0x> <%0x> <%0x> <%0x> \n" , __st45LP->wBuf[0] 
            , __ch[0] , __ch[1] , __ch[2] , __ch[3] ) ;
    gap01();

    __ch = 
        i2c_reg_read_bytes( __st45LP, 0x08, 4 ) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918385 : get from Reg <0x%0x> : <%0x> <%0x> <%0x> <%0x> \n" , __st45LP->wBuf[0] 
            , __ch[0] , __ch[1] , __ch[2] , __ch[3] ) ;
    gap01();

    return true ;
} // stusb4500_read_byte_test
