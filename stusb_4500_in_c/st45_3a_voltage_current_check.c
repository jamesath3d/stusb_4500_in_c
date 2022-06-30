#include "st45_00_all.h"

void _st45_check_and_modify_range_V( ST45pdo* ___pdo, float* ___V ){
    if ( *___V < 5.0 ) {
        printf ("83818818181 : voltage check failed : pdo %.2f , force modify to 5.0V <<<<<==========\n" , *___V );
        *___V = 5.0 ;
        return ;
    }
    if ( *___V > 20.0 ) {
        printf ("83818818183 : voltage check failed : pdo %.2f , force modify to 20.0V <<<<<==========\n" , *___V );
        *___V = 20.0 ;
    }
    if ( ___pdo ) {
        ___pdo -> Vf = *___V ;
        ___pdo -> Vu = 20 * (*___V) ; // 50mV for 1
    }
} // _st45_check_and_modify_range_V

void _st45_check_and_modify_range_I( ST45pdo* ___pdo, float* ___I ){
    // note : according to the manual, when the Iconfig Reg is 0, turn to use the I_SNK_PDO_FLEX[9:0], max 2.0A
    if ( *___I < 0.5 ) {
        printf ("83818818185 : current check failed : pdo %.2f , force modify to 0.5A <<<<<==========\n" , *___I );
        *___I = 0.5 ;
    }
    if ( *___I > 5.0 ) {
        printf ("83818818187 : current check failed : pdo %.2f , force modify to 5.0A <<<<<==========\n" , *___I );
        *___I = 5.0 ;
    }
    if ( ___pdo ) {
        ___pdo -> If = *___I ;
        ___pdo -> Iu = 20 * (*___I) ; // 50mV for 1
    }
} // _st45_check_and_modify_range_I

void _st45_check_and_fill_config( ST45pdo* ___pdo, float ___V, float ___I ) {
    _st45_check_and_modify_range_V( ___pdo, &___V ) ;
    _st45_check_and_modify_range_I( ___pdo, &___I ) ;
} // _st45_check_and_fill_config
