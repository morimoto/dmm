/************************************************************************


                      memctrl

2009/04  morimoto
************************************************************************/
#ifndef MEM_H
#define MEM_H

#include "dmm.h"

//=====================================
//
//          ESTYPE
//
//=====================================
enum ESTYPE {
    TBYTE,
    TWORD,
    TLONG,
    TERR,
};

//=====================================
//
//          STADDRMAP
//
//=====================================
typedef struct _addrmap {
    u32   nSize;
  uintptr_t   rMap;
  uintptr_t   rTgt;
} STADDRMAP;

//=====================================
//
//          STMEMCTRL
//
//=====================================
typedef struct _memctrl {
  bool   (*fnEdit)( uintptr_t, void* );
  bool   (*fnRead)( uintptr_t, u32* );
  bool   (*fnDump)( uintptr_t );
    u32   nIncSize;
    enum ESTYPE   EStype;
} STMEMCTRL;

STMEMCTRL* GetMemCtrl( enum ESTYPE );

bool       MemInit( void );
void       MemExit( void );

//----------------------
// system function
//----------------------
bool _sysMemCtrlInit( void );


#endif
