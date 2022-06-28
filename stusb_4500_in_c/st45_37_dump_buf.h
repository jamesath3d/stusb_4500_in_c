
#ifndef  ST45_37_DUMP_BUF_H__
#define  ST45_37_DUMP_BUF_H__

#include "st45_00_all.h"

void _st45_dump_buf( char* ___cmpBuf , char* ___pre, char* ___pre2, char* ___tail ) ;
#define ST45_dump_buf3( buf, preX, tail) _st45_dump_buf( (char*) buf , (char*)__func__, preX, tail )
#define ST45_dump_buf2( buf, preX )      _st45_dump_buf( (char*) buf , (char*)__func__, preX, "=" )
#define ST45_dump_buf1( buf )            _st45_dump_buf( (char*) buf , (char*)__func__, "#",  "#" )

#endif

