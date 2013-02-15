/************************************************************************


                                    cpu

This file is loaded from cpu/${ARCH} file

2008/11  morimoto
************************************************************************/
#include "cmd/cmdlist.h"

static const STCPU *s_pCpu = NULL;

//======================================================================
//
//
//                      local function , variable
//
//
//======================================================================

//=====================================
//
//          SetCpu
//
//=====================================
static void SetCPU( const STCPU *pCpu )
{
    s_pCpu = pCpu;
}

//======================================================================
//
//
//                      global function
//
//
//======================================================================

//=====================================
//
//          GetCPUList
//
//=====================================
const STCPU* GetCPUList( int ixIndex )
{
    if ( 0              >  ixIndex ||
         s_nCpuListSize <= ixIndex )
        return NULL;

    return s_ptblCpuList[ixIndex];
}

//=====================================
//
//          GetCPUListSize
//
//=====================================
int GetCPUListSize( void )
{
    return s_nCpuListSize;
}

//=====================================
//
//          GetCPU
//
//=====================================
const STCPU* GetCPU( void )
{
    return s_pCpu;
}

//=====================================
//
//          CPUInit
//
//=====================================
static void archinit( void );
void _sysCPUInit( void )
{
    //----------------------
    // default enable cmd
    //----------------------
    EnableCmd( "md" );
    EnableCmd( "me" );
    EnableCmd( "help" );
    EnableCmd( "version" );

    archinit( );
}

//=====================================
//
//          _sysArchCmdInit
//
//=====================================
void _sysArchCmdInit( const STCMD *pCmd )
{
    GetCPU(  )->fnCmdInit( pCmd );
}
