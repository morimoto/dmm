/************************************************************************


                                  md

2008/09  morimoto
************************************************************************/
#include "dmm.h"

//=====================================
//
//          help
//
//=====================================
static STHELPMSG s_Help[] = {
    {
        "md" , "memory dump" ,
        "dmm md[.d, .w, .l] addr [len]\n"
        "    addr  : address\n"
        "    len   : length\n"
        " ex) dmm md.b 0x88000000\n"
        "     dmm md.w 0x88000000 0x20\n"
        "     dmm md   0x88000ff0 32    # it mean LONG"
    },
    HELPMSGEND
};

//=====================================
//
//          mem_dump
//
//=====================================
static int mem_dump_single_column( STMEMCTRL *pCtrl ,
                                   u32 ulAddr ,
                                   u32 ulLen )
{
    u32 inc;
    u32 l;

    DMSG( "%s\n" , __FUNCTION__);

    switch ( pCtrl->EStype ) {
    case TBYTE : inc = 1; break;
    case TWORD : inc = 2; break;
    case TLONG : inc = 4; break;
    case TERR:
    default: printf( "unknown stype\n" ); return false;
    }

    for ( l=ulAddr ; l<ulAddr + ulLen ; l+=inc ) {

        BLUE;
        printf( "0x%08lx   " , l );
        CLAR;

        if( !pCtrl->fnDump( l ))
            break;
        printf( "\n" );
    }

    return true;
}

static int mem_dump_mult_column( STMEMCTRL *pCtrl ,
                                  u32 ulAddr ,
                                  u32 ulLen )
{
    u32         i      = 0;
    u32         l      = 0;
    const char* space  = NULL;

    DMSG( "%s\n" , __FUNCTION__);

    //----------------------
    // draw offset line
    //----------------------
    BLUE;
    printf("             ");
    switch ( pCtrl->EStype ) {
    case TBYTE : space = " " ;      break;
    case TWORD : space = "   ";     break;
    case TLONG : space = "       "; break;
    case TERR:
    default: printf( "dump error\n" ); return false;
    }

    for ( i=0 ; i<=0xF ; i+=pCtrl->nIncSize )
        printf( "%lx%s " , i , space );

    CLAR;
    printf( "\n" );

    //----------------------
    // draw memory
    //----------------------
    for ( l=(ulAddr & 0xFFFFFFF0) ; l<ulAddr + ulLen ; l+=0x10 ) {

        BLUE;
        printf( "0x%08lx   " , l );
        CLAR;

        for ( i=0 ; i<=0xF ; i+=pCtrl->nIncSize ) {

            if (( l + i ) >= ulAddr+ulLen )
                break;

            if (( l + i ) <  ulAddr ) {
                printf( " %s " , space );
            }
            else {
                if( !pCtrl->fnDump( l+i ))
                    break;
                printf( " " );
            }

        }
        printf( "\n" );
    }

    return true;
}

//=====================================
//
//                cmd
//
//=====================================
static bool cmd( int nArgc, char *pstrArgv[] )
{
    u32           addr   = GetAddress( pstrArgv[1] );
    enum ESTYPE   stype  = CmdGetDataSize( pstrArgv[0] , TLONG );
    STMEMCTRL*    pmctrl = NULL;
    bool          ret    = false;

    if ( TERR == stype )
        return Usage( "type error" );

    if( !MemInit(  ))
        return Error( "mem ctrl init error" );

    pmctrl = GetMemCtrl( stype );
    if ( !pmctrl ) {
        Error( "get mem ctrl error" );
        goto error;
    }

    if ( addr & ( pmctrl->nIncSize-1 )) {
            Error( "stype and addr error" );
            goto error;
    }

    if ( 2 == nArgc )
        ret = mem_dump_single_column( pmctrl , addr , 1 );
    else
        ret = mem_dump_mult_column( pmctrl , addr , GetData( pstrArgv[2] ));

 error:
    MemExit(  );

    return ret;
}

//=====================================
//
//          ishit
//
//=====================================
static bool
ishit( int nArgc, char *pstrArgv[] )
{
    if ( (CMD_HIT( pstrArgv[0] , "md.b" ) ||
          CMD_HIT( pstrArgv[0] , "md.w" ) ||
          CMD_HIT( pstrArgv[0] , "md.l" ) ||
          IsCmdHit( ))
         &&
         (( 2 == nArgc ) || ( 3 == nArgc )))
        return true;

    return false;
}
INSTALL_CMD( md )
