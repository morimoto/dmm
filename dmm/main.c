/************************************************************************

 dmm
	2008/09/03  Kuninori Morimoto   renovation start
	2005/01/26  Sano hideyuki       develop start

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License version 2 as
 published by the Free Software Foundation.

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
    if( 2 > nArgc )
        return Usage( "parameter is nedded" );

    // shift arg
    nArgc--;
    pstrArgv++;

    for ( i=0 ; i<GetCmdListSize( ) ; i++ ) {

        cmd = GetCmdList( i );

        if ( CMD_HIT( pstrArgv[0], cmd->pstrName ) &&
             pstrArgv[1] &&
             (CMD_HIT( pstrArgv[1], "--help" ) ||
              CMD_HIT( pstrArgv[1], "-h" )))
            return DetailUsage( cmd->pstrName );

        if ( cmd->fnIsHit( nArgc , pstrArgv ) )
            goto cmdstart;
    }

    return
        Usage( "parameter error" );

 cmdstart:
    if( !cmd->fnCmd( nArgc , pstrArgv ) )
        Error( "Cmd return error" );

    return 0;
}
