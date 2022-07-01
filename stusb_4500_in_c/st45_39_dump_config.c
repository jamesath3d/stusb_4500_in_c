#include "st45_00_all.h"

void _st45_dump_st45pdo( ST45config* ___nvmConfig , char ___pdoIdx , char* ___pr ) {
    ST45pdo* __pdo ;
    __pdo = &(___nvmConfig -> pdo[0]) ;
    __pdo += ___pdoIdx - 1;

    printf( "%s PDO %d V,I is : 0x%03X-%5.2fv ,  0x%1X-%4.2fA , lowVp: %d%%, upVp: %d%% \n", ___pr
            , ___pdoIdx
            , __pdo -> Vu , __pdo -> Vf
            , __pdo -> Iu , __pdo -> If
            , __pdo -> lowerVpercent , __pdo -> upperVpercent
          );

} // _st45_dump_st45pdo

void _st45_dump_st45config( ST45config* ___nvmConfig , char * ___pr ) {
    printf( "%s PDO amount is : %d , FlexCurrent is 0x%02X-%f \n", ___pr
            , ___nvmConfig -> pdoAmount 
            , ___nvmConfig -> flexCurrentU
            , ___nvmConfig -> flexCurrentF 
            );
    _st45_dump_st45pdo( ___nvmConfig, 1 , ___pr );
    _st45_dump_st45pdo( ___nvmConfig, 2 , ___pr );
    _st45_dump_st45pdo( ___nvmConfig, 3 , ___pr );
} // _st45_dump_st45config


