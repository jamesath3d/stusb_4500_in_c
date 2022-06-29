#include "st45_00_all.h"

void _st45_dump_st45pdo( ST45config* ___nvmConfig , char ___pdoIdx ) {
    ST45pdo* __pdo ;
    __pdo = &(___nvmConfig -> pdo1) ;
    __pdo += ___pdoIdx - 1;

    printf( " PDO %d V,I is : 0x%03X-%5.2fv ,  0x%1X-%4.2fA \n"
            , ___pdoIdx
            , __pdo -> Vu , __pdo -> Vf
            , __pdo -> Iu , __pdo -> If
          );

} // _st45_dump_st45pdo

void _st45_dump_st45config( ST45config* ___nvmConfig ) {
    printf( " PDO amount is : %d \n", ___nvmConfig -> pdoAmount );
    _st45_dump_st45pdo( ___nvmConfig, 1 );
    _st45_dump_st45pdo( ___nvmConfig, 2 );
    _st45_dump_st45pdo( ___nvmConfig, 3 );
} // _st45_dump_st45config