#include "st45_00_all.h"

void _st45_check_and_modify_range_V( ST45pdo* ___pdo, float* ___V ){
    if ( *___V < 5.0 ) {
        printf ("83818818181 : voltage check failed : pdo %.2f , force modify to 5.0V \n" , *___V );
        *___V = 5.0 ;
        return ;
    }
    if ( *___V > 20.0 ) {
        printf ("83818818183 : voltage check failed : pdo %.2f , force modify to 20.0V \n" , *___V );
        *___V = 20.0 ;
    }
} // _st45_check_and_modify_range_V

void _st45_check_and_modify_range_I( ST45pdo* ___pdo, float* ___I ){
    // note : according to the manual, when the Iconfig Reg is 0, turn to use the I_SNK_PDO_FLEX[9:0], max 2.0A
    if ( *___I < 0.5 ) {
        printf ("83818818185 : current check failed : pdo %.2f , force modify to 0.5A \n" , *___I );
        *___I = 0.5 ;
    }
    if ( *___I > 5.0 ) {
        printf ("83818818187 : current check failed : pdo %.2f , force modify to 5.0A \n" , *___I );
        *___I = 5.0 ;
    }
} // _st45_check_and_modify_range_I

void _st45_check_and_fill_config( ST45pdo* ___pdo, float ___V, float ___I ) {
    _st45_check_and_modify_range_V( ___pdo, &___V ) ;
    _st45_check_and_modify_range_I( ___pdo, &___I ) ;
    //_st45newConfig . pdo[1] . Vf = __Vconfig2 ;
    //_st45newConfig . pdo[1] . Vu = __Vconfig2 * 20 ;
    //_st45newConfig . pdo[0] . If = __Iconfig1 ;
    //_st45newConfig . pdo[0] . Iu = __Iconfig1 * 20 ;
} // _st45_check_and_fill_config
