/************************************************************************


                                    cpu.h

2008/11  morimoto
************************************************************************/
#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

//=====================================
//
//          STCPU
//
//=====================================
typedef struct _cpu {
    const char      *pstrName;
    void (*fnInit)(const struct _cpu*);
    void (*fnCmdInit)(const STCMD*);
} STCPU;

//=====================================
//
//           define
//
//=====================================
#define CPU_EXTERN(name) extern const STCPU cpu_##name;
#define CPU_DEFINE(name) &cpu_##name,
#define CPU_LIST_SIZE   static int s_nCpuListSize = ARRAY_SIZE( s_ptblCpuList )

#define START(a)
#define END(a)
 
#define INSTALL_CPU(name) const STCPU cpu_##name = { \
        .pstrName  = #name,                    \
        .fnInit    = init,                     \
        .fnCmdInit = cmdinit,                  \
};

//======================================================================
//
//
//                        function
//
//
//======================================================================
const STCPU*	GetCPUList( int );
const STCPU*	GetCPU( void );
int		GetCPUListSize( void );

//----------------------
// system function
//----------------------
void _sysCPUInit( void );
void _sysArchCmdInit( const STCMD* );

#endif /* CPU_H */
