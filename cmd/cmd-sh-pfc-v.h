/************************************************************************


                               cmd-sh-pfc-v.h

2009/04  morimoto
************************************************************************/
#ifndef CMD_SH_PFC_V_H
#define CMD_SH_PFC_V_H

#include "dmm.h"
#include "regctrl.h"

//----------------------
// STPFCSEL
//----------------------
typedef struct _pfc_sel_ {
    const char *pFncName;
    int nRegPos;
    int nValPos;
} STPFCSEL;

#define PFC_FN(name) { .pFncName = name, .nRegPos = -1, .nValPos = -1 }
#define PFC_FNS(reg, val) { .pFncName = NULL, .nRegPos = reg, .nValPos = val }

//----------------------
// STPFCCTRL
//----------------------
typedef struct _pfcv {
    REGID     nID;
    STPFCSEL *pMember;
} STPFCCTRL;

#define SETPFC( id, mem...) {      \
        .nID      = id,            \
        .pMember  = (STPFCSEL[]) { \
            mem,                   \
            { NULL, 0 , 0 }        \
        }}

//----------------------
// STSHPFCCTRL
//----------------------
typedef struct _pfc_ctrl_ {
    STPFCCTRL *pPxCR;
    int        nPxCRSize;
    STREGCTRL *pPSEL; 
    int        nPSELSize;
} STSHPFCCTRL;

#endif /* CMD_SH_PFC_V_H */
