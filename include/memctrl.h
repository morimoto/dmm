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
    u32   rMap;
    u32   rTgt;
} STADDRMAP;

//=====================================
//
//          STMEMCTRL
//
//=====================================
typedef struct _memctrl {
    bool   (*fnEdit)( u32, void* );
    bool   (*fnRead)( u32, u32* );
    bool   (*fnDump)( u32 );
    u32   nIncSize;
    enum ESTYPE   EStype;
} STMEMCTRL;

STADDRMAP* GetAddrMap( u32 , STMEMCTRL* );
STMEMCTRL* GetMemCtrl( enum ESTYPE );

bool       MemInit( void );
void       MemExit( void );

//----------------------
// system function
//----------------------
bool _sysMemCtrlInit( void );


#endif
