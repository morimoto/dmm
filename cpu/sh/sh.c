/************************************************************************


                                    sh

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/cpulist.c"
#include "../cpu.c"

//======================================================================
//
//
//                               define
//
//
//======================================================================
#define CPUINFO "/proc/cpuinfo"
#define CPUTYPE "cpu type"
#define MAXBUF 1024

//======================================================================
//
//
//                      local function , variable
//
//
//======================================================================

//=====================================
//
//          check_cpu
//
//=====================================
static const STCPU* check_cpu( void )
{
    const STCPU     *pcpu = NULL;
    FILE            *cpuf = fopen( CPUINFO , "r" );
    char             buf[MAXBUF];
    char            *pbuf;

    if ( !cpuf ) { Error( "cpuinfo open error" ); return NULL; }

    while ( fgets( buf , MAXBUF , cpuf ) ) {
        if ( 0 == strncmp( buf , CPUTYPE , strlen(CPUTYPE) )) {

            int i;

            // this time buf is..
            // type1        : value1
            // type2        : value2...
            pbuf = strstr( buf , ":" );
            if ( !pbuf ) { Error( "is this real sh?" ); goto error;}

            pbuf++;  // :
            pbuf++;  // space

            pbuf[strlen( pbuf )-1] = '\0';

            for ( i=0 ; i<s_nCpuListSize ; i++ ) {
                if ( 0 == strcmp( pbuf , s_ptblCpuList[i]->pstrName )) {
                    pcpu = s_ptblCpuList[i];
                    pcpu->fnInit( pcpu );
                    break;
                }
            }
            break;
        }
    }

 error:
    fclose( cpuf );

    return pcpu;
}

//=====================================
//
//          archinit
//
//=====================================
static void archinit( void )
{
    const STCPU *cpu = NULL;

    cpu = check_cpu(  );

    if ( !cpu )
        Error( "sh sub cpu type error" );

    //----------------------
    // default enable cmd for sh
    //----------------------
    EnableCmd( "cpu" );

    SetCPU( cpu );
}
