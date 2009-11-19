/************************************************************************


                      cpu ( if cpu is sh )


************************************************************************/
#include "dmm.h"
#include "cpu.h"

static STHELPMSG s_Help[] = {
    {"cpu"        , "print cpu type (if supported)" , "dmm cpu" },
    {"cpusupport" , "print supported cpu type"      , "dmm cpusupport" },
    HELPMSGEND
};

//=====================================
//
//               cmd
//
//=====================================
static bool cmd( int nArgc, char *pstrArgv[] )
{
    const STCPU   *cpu = GetCPU(  );
    int            i   = 0;

    if( !cpu ) {
        printf( "Unknown SH CPU\n" );
        return false;
    }

    if ( CMD_HIT( "cpu" , pstrArgv[0] )) {
        printf( "cpu     : %s\n" , cpu->pstrName );
        printf( "support : " );
        for ( i=0 ; i<GetCmdListSize( ) ; i++ ) {
            if ( IsEnableCmd( i )) {
                printf( "%s, " , GetCmdList( i )->pstrName );
            }
        }
        printf( "\n" );
        return true;
    }

    if ( CMD_HIT( "cpusupport" , pstrArgv[0] )) {
        int size = GetCPUListSize(  );
        for ( i=0 ; i<size ; i++ )
            printf("%s\n" , GetCPUList( i )->pstrName );
    }

    return true;
}

//=====================================
//
//          ishit
//
//=====================================
static bool
ishit( int nArgc, char *pstrArgv[] )
{

    if ( IsCmdHit( ))
        return true;

    return false;
}
INSTALL_CMD( cpu )
