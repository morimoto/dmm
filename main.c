/************************************************************************

                  memory manage for develop

	2008/09/03  Kuninori Morimoto   renovation start
	2005/01/26  Sano hideyuki       develop start

************************************************************************/
//=====================================
//
//             include
//
//=====================================
#include "dmm.h"

//=====================================
//
//               define
//
//=====================================

//======================================================================
//
//
//                                function
//
//
//======================================================================
//=====================================
//
//               main
//
//=====================================
int main( int   nArgc,
	  char *pstrArgv[] )
{
    int i;
    const STCMD* cmd;

    //=====================================
    //
    //             check param
    //
    //=====================================
    _sysCPUInit(  );

    if( 2 > nArgc )
        return Usage( "parameter is nedded" );

    // shift arg
    nArgc--;
    pstrArgv++;

    for ( i=0 ; i<GetCmdListSize( ) ; i++ ) {

        if ( !IsEnableCmd( i ) )
            continue;

        cmd = GetCmdList( i ); 
        if ( cmd->fnIsHit( nArgc , pstrArgv ) )
            goto cmdstart;
    }

    return
        Usage( "parameter error" );

 cmdstart:
    _sysArchCmdInit( cmd );

    if( !cmd->fnCmd( nArgc , pstrArgv ) )
        Error( "Cmd return error" );

    return 0;
}
