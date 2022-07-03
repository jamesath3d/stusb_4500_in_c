
#ifndef  I2C_ALL_H__
#define  I2C_ALL_H__

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <stdio.h>				//printf
#include <stdlib.h>				//printf
#include <stdbool.h>            // bool
#include <stdint.h>             // uint8_t
#include <string.h>             // memcpy

#define Fprintf( fmt, ... ) printf( "%s: " fmt, __func__, __VA_ARGS__)
#define FP Fprintf
#define P printf
#define FP1(fmt) printf("%s: " fmt, __func__)
#define EXi(rtI) {printf("Error met : %s line %d, return %d \n" , \
        __func__, __LINE__, rtI ); \
    exit( -1 ) ; \
    return rtI ; }
#define EXb(rtB) {printf("Error met : %s line %d, return %s \n" , \
        __func__, __LINE__, (rtB?"true":"false") ); \
    exit( -1 ) ; \
    return rtB ; }
#define EXlp(rtLP) {printf("Error met : %s line %d, return 0x%0X \n" , \
        __func__, __LINE__, 0 ); \
    exit( -1 ) ; \
    return rtLP ; }

struct ST45i2cST {
    uint8_t     i2cBusNo ;
    char        i2cBusName[256] ;
    uint8_t     i2cClientAddr ;
    char        wBuf[80];
    char        rBuf[80];
    size_t      wLEN ;
    size_t      rLEN ;
    int         wRT ;
    int         rRT ;
    int         i2cFileNO ;
} ;
typedef struct ST45i2cST ST45i2cST ;

struct ST45pdo {
    uint16_t    Vu ;
    float       Vf ;
    uint16_t    Iu ;
    float       If ;
    uint16_t    lowerVpercent ; 
    uint16_t    upperVpercent ;
} ;
typedef struct ST45pdo ST45pdo ;

struct ST45config {
    uint8_t     pdoAmount ;
    ST45pdo     pdo[3] ;
    uint16_t    flexCurrentU ;
    float       flexCurrentF ;
    uint8_t     buf[5][8] ;
} ;
typedef struct ST45config ST45config ;

#include "i2c_11_bus_init.h"
#include "i2c_21_reg_read_write_test.h"
#include "i2c_23_reg_write.h"
#include "i2c_31_tx.h"

#endif

