/************************************************************************


                                  help

2008/09  morimoto
************************************************************************/
#include "dmm.h"

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
    //----------------------
    // if --help or -h
    // print Usage only
    //----------------------
    if( CMD_HIT( "--help" , pstrArgv[0] ) ||
        CMD_HIT( "-h"     , pstrArgv[0] ) ||
        nArgc < 2 ) {
        Usage( "help" );
        return true;
    }

    //----------------------
    // if help command is used,
    // print detail
    //----------------------
    return DetailUsage( pstrArgv[1] );
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
