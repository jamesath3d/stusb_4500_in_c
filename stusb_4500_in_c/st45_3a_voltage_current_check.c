#include "st45_00_all.h"

void _st45_check_and_modifyV( uint8_t ___pdoIdx, float* ___V ){
    if ( *___V < 5.0 ) {
        printf ("83818818181 : voltage check failed : pdo %d, %.2f , force modify to 5.0V \n" , 
                ___pdoIdx, *___V );
        *___V = 5.0 ;
    }
    if ( *___V > 20.0 ) {
        printf ("83818818183 : voltage check failed : pdo %d, %.2f , force modify to 20.0V \n" , 
                ___pdoIdx, *___V );
        *___V = 20.0 ;
    }
} // _st45_check_and_modifyV

void _st45_check_and_modifyI( uint8_t ___pdoIdx, float* ___I ){
    if ( *___I < 0.5 ) {
        printf ("83818818185 : current check failed : pdo %d, %.2f , force modify to 0.5A \n" , 
                ___pdoIdx, *___I );
        *___I = 0.5 ;
    }
    if ( *___I > 5.0 ) {
        printf ("83818818187 : current check failed : pdo %d, %.2f , force modify to 5.0A \n" , 
                ___pdoIdx, *___I );
        *___I = 5.0 ;
    }
} // _st45_check_and_modifyI
