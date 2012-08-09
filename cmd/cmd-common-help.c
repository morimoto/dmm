/************************************************************************


                                  help

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include "cpu.h"

//=====================================
//
//          help
//
//=====================================
static STHELPMSG s_Help[] = {
    {"--help" , "print help (-h)"  , "dmm --help"},
    {"-h"     , "print help"       , "dmm -h"},
    {"help"   , "print detail (dmm help md)"     , "dmm help [cmd]"},
    HELPMSGEND
};

//=====================================
//
//               cmd
//
// argv[0] -> command
// argv[1] -> args
//=====================================
static bool cmd( int nArgc, char *pstrArgv[] )
{
    int        i     = 0;
    STHELPMSG *phelp = NULL;
    bool       ret   = false;

    //----------------------
    // if --help or -h
    // print Usage only
    //----------------------
    if( CMD_HIT( "--help" , pstrArgv[0] ) ||
        CMD_HIT( "-h"     , pstrArgv[0] ) ||
        nArgc < 2 ) {
        ret = true;
        Usage( "help" );
        goto end;
    }

    //----------------------
    // if help command is used,
    // print detail
    //----------------------
    for( i=0 ; i<GetCmdListSize( ); i++ ) {
        if( !IsEnableCmd( i ) )
            continue;

        phelp = GetCmdList( i )->pHelp;
        while ( phelp->pstrCmd ) {
            if( CMD_HIT( phelp->pstrCmd , pstrArgv[1] )) {
                printf( "%s\n" , phelp->pstrDetail );
                ret = true;
                goto end;
            }
            phelp++;
        }
    }

 end:

    if ( !ret )
        Usage( "help" );

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
    if ( IsCmdHit( ))
        return true;

    return false;
}
INSTALL_CMD( help )
