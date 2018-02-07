/*=============================================

                     stdlib

2008/09  morimoto
==============================================*/
#include "dmm.h"

//=====================================
//
//              GetAddress
//
//=====================================
u32 GetAddress( const char *pstrArgv )
{
    u32 addr = 0;

    sscanf( pstrArgv , "%lx" , &addr );
    DMSG( "addr = 0x%08lx\n" , addr );

    return addr;
}

//=====================================
//
//          GetStype
//
//=====================================
enum ESTYPE GetStype( const char *pstrArgv )
{
    enum ESTYPE type = TERR;

    DMSG( "type = " );

    switch ( pstrArgv[0] ) {
    case 'b' : type = TBYTE; DMSG( "BYTE\n" ); break;
    case 'w' : type = TWORD; DMSG( "WORD\n" ); break;
    case 'l' : type = TLONG; DMSG( "LONG\n" ); break;
    default: DMSG( "type error\n" );
    }

    if ( 1 != strlen( pstrArgv ) )
        type = TERR;

    return type;
}

//=====================================
//
//          GetData
//
//=====================================
u32 GetData( const char *pstrArgv )
{
    u32  ret = 0;
    const char    *fmt = "%d";

    //----------------------
    // check if argv 0xAA style
    //----------------------
    if ((2 < strlen( pstrArgv )) &&
        ( '0' == pstrArgv[0]   ) &&
        ( 'x' == pstrArgv[1]   )) {
        fmt = "%x";
    }

    sscanf( pstrArgv , fmt , &ret );
    DMSG( "length (10)%d , (16)0x%x\n" , ret , ret );

    return ret;
}

//=====================================
//
//                Error
//
//=====================================
bool _Error( const char *pstrError , const char *pstrFunction , int nLine )
{
    printf( "[error %15s :: %-5d] : %s\n" ,
            pstrFunction,
            nLine,
            pstrError
            );
    return false;
}
