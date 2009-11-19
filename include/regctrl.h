/************************************************************************


                                    regctrl.h

2009/04  morimoto
************************************************************************/
#ifndef REGCTRL_H
#define REGCTRL_H

#include "dmm.h"

//----------------------
// STREG
//----------------------
typedef struct _m_reg {
    const char  *pstrName;
    int          nSize;
    const char **ppExplain;
} STREG;

//----------------------
// STREGCTRL
//----------------------
typedef struct _regctrl {
    REGID        nID;
    u32          ccVal;
    enum ESTYPE  EType;
    STREG       *pMember;
} STREGCTRL;

#define SETREG(id, type, mem...) { \
    .nID      = id,                \
    .EType    = type,              \
    .pMember  = (STREG[]) {        \
        mem,                       \
        { NULL, 0 , NULL }         \
    }}

#define DREG(name, len) { .pstrName = name, .nSize = len }
#define EREG(name, len, exp...) { .pstrName = name, .nSize = len, .ppExplain = (const char*[]){exp} }

//======================================================================
//
//
//                        function
//
//
//======================================================================
bool RegSyncRead( STREGCTRL* );
bool RegSyncWrite( STREGCTRL* );
u32  GetRegVal( STREGCTRL*, int );
u32  SetRegVal( STREGCTRL*, int, u32 );
const char* GetRegName( REGID );
const char* GetRegCmdName( REGID );
REGID GetRegID( const char* , REGID , REGID );
u32  GetRegAddr( REGID );
bool  RegCheck( REGID );
const char *GetRegExplain( STREGCTRL*, int);
void SetRegs( int , u32 );

//----------------------
// system function
//----------------------
void _sysRegInit( void );


#endif /* REGCTRL_H */
