/************************************************************************


                                    none

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include "cpu.h"


static void cmdinit( const STCMD *pCmd )
{
}

static void init( const STCPU *pCpu )
{
}

INSTALL_CPU( NONECPU )

static const STCPU *s_ptblCpuList[] = {
    &cpu_NONECPU,
};
CPU_LIST_SIZE;

#include "../cpu.c"                                                     \

//=====================================
//
//          archinit
//
//=====================================
static void archinit( void )
{
    SetCPU( s_ptblCpuList[0] );
}
