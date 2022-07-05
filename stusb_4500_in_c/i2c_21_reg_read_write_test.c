#include "i2c_00_all.h"

// ST45default
void _prDefaultRegsOfStusb4500(void){
    P( "\n i2cdump -y 9 0x28 \n ------------- Should got -------------- \n" );
} // _prDefaultRegsOfStusb4500

inline void gap01(void){
    usleep(1000);
    printf("\n");
} // gap01


bool stusb4500_read_byte_test( ST45i2cST * __st45LP ){
    uint8_t * __ch ;

    if ( NULL == __st45LP ) EXb( false );
    __ch = 
        _i2c_reg_read_one_byte( __st45LP, 0x06) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918381 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        _i2c_reg_read_one_byte( __st45LP, 0x06) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        _i2c_reg_read_one_byte( __st45LP, 0x08) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        _i2c_reg_read_one_byte( __st45LP, 0x08) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918383 : get from Reg <0x%0x> : <%0x> \n" , __st45LP->wBuf[0] , __ch[0] ) ;
    gap01();

    __ch = 
        _i2c_reg_read_bytes( __st45LP, 0x08, 4 ) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918385 : get from Reg <0x%0x> : <%0x> <%0x> <%0x> <%0x> \n" , __st45LP->wBuf[0] 
            , __ch[0] , __ch[1] , __ch[2] , __ch[3] ) ;
    gap01();

    __ch = 
        _i2c_reg_read_bytes( __st45LP, 0x06, 4 ) ;
    if ( NULL == __ch ) return false ;
    printf( " 81939918385 : get from Reg <0x%0x> : <%0x> <%0x> <%0x> <%0x> \n" , __st45LP->wBuf[0] 
            , __ch[0] , __ch[1] , __ch[2] , __ch[3] ) ;
    gap01();

    if ( (0x12 != __ch[0]) || (0x11 != __ch[2]) ) {
        FP( "Error found: Reg 0x06 get 0x12, Reg 0x08 get 0x11 ! ERROR ! \n"
                "you can try to test the i2c use the following commands : \n\n"
                "i2cdump -y %d %d \n"
                "i2cdetect -y %d \n\n",
                __st45LP -> i2cBusNo,
                __st45LP -> i2cClientAddr,
                __st45LP -> i2cBusNo
           );
        EXb( false );
    }


    return true ;
} // stusb4500_read_byte_test
