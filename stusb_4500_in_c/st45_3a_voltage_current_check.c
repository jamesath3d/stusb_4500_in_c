#include "st45_00_all.h"

float _checkFloat( char* ___info, float ___F , float ___low, float ___up ){
    if ( ___F < ___low ) {
        printf ( ___info, ___F ) ;
        ___F = ___low ;
    }
    if ( ___F > ___up ) {
        printf ( ___info, ___F ) ;
        ___F = ___up ;
    }
    return ___F ;
} // _checkFloat

uint8_t _checkUint8( char* ___info, uint8_t ___F , uint8_t ___low, uint8_t ___up ){
    if ( ___F < ___low ) {
        printf ( ___info, ___F ) ;
        ___F = ___low ;
    }
    if ( ___F > ___up ) {
        printf ( ___info, ___F ) ;
        ___F = ___up ;
    }
    return ___F ;
} // _checkUint8

void _st45_check_and_modify_range_V( ST45pdo* ___pdo, float ___V ){
    ___V = _checkFloat(
            "83818818181 : voltage check failed : pdo %.2f , force modify to 20.0V <<<<<==========\n" , ___V , 5.0, 20.0 );
    if ( ___pdo ) {
        if ( ___V < 0 )     ___V = 0 ;
        if ( ___V > 20 )    ___V = 20 ;
        ___pdo -> Vf = ___V ;
        ___pdo -> Vu = 20 * (___V) ; // 50mV for 1
        ___pdo -> Vf = 0.05 * ___pdo -> Vu ; // to prevent the input float is not good fit
        if ( (___pdo -> Vu) != ( 0x3FF & (___pdo -> Vu)) ) {
            printf( "83818818182 : error found : Vu : 0x%03X out of range 0x3FF \n" , ___pdo -> Vu ) ;
        }
    }
} // _st45_check_and_modify_range_V

void _st45_check_and_modify_range_I( ST45pdo* ___pdo, float ___I ){
    // note : according to the manual, when the Iconfig Reg is 0, turn to use the I_SNK_PDO_FLEX[9:0], max 2.0A
    ___I = _checkFloat(
            "83818818183 : current check failed : pdo %.2f , force modify to 0.5A <<<<<==========\n" , ___I , 0.5, 5.0 );
    if ( ___pdo ) {
        if ( ___I < 0 )     ___I = 0 ;
        if ( ___I > 5 )     ___I = 5 ;
        ___pdo -> If = ___I ;
        if ( ___I == 0 ) {
            ___pdo -> Iu = 0 ;
            ___pdo -> If = 0 ;
        } else if ( ___I <= 3.0 ) {
            ___pdo -> Iu = (___I - 0.25)/0.25 ;
            ___pdo -> If = 0.25 + (0.25 * (___pdo -> Iu)) ; // to prevent the input float is not good fit
        } else {
            ___pdo -> Iu = 0b1011 + (___I - 3)/0.5 ;
            ___pdo -> If = 3 + (0.5 * (___pdo -> Iu - 0b1011 )) ; // to prevent the input float is not good fit
        }
        if ( (___pdo -> Iu) != ( 0xF & (___pdo -> Iu)) ) {
            printf( "83818818184 : error found : Iu : 0x%2X out of range 0xF \n" , ___pdo -> Iu ) ;
        }
    }
} // _st45_check_and_modify_range_I

void _st45_check_and_modify_range_lower_up_V( ST45pdo* ___pdo, uint8_t ___low, uint8_t ___up ){
    ___low = _checkUint8(
            "83818818185 : current check failed : pdo %.2f , force modify to 0.5A <<<<<==========\n" , ___low, 1, 15 );
    ___up = _checkUint8(
            "83818818186 : current check failed : pdo %.2f , force modify to 0.5A <<<<<==========\n" , ___up, 1, 15 );
    if ( ___pdo ) {
        ___pdo -> lowerVpercent = ___low ;
        ___pdo -> upperVpercent = ___up ;
        if ( (___low) != ( 0xF & (___low)) ) {
            printf( "83818818187 : error found : lowerV : 0x%2X out of range 0xF \n" , ___low ) ;
        }
        if ( (___up) != ( 0xF & (___up)) ) {
            printf( "83818818188 : error found : upperV : 0x%2X out of range 0xF \n" , ___up ) ;
        }
    }
} // _st45_check_and_modify_range_I

void _st45_check_and_fill_config( ST45pdo* ___pdo, float ___V, float ___I , uint8_t ___lowVp, uint8_t ___upVp ) {
    _st45_check_and_modify_range_V(         ___pdo, ___V        ) ;
    _st45_check_and_modify_range_I(         ___pdo, ___I        ) ;
    _st45_check_and_modify_range_lower_up_V(   ___pdo, ___lowVp, ___upVp );
} // _st45_check_and_fill_config
