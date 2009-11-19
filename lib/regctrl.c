/************************************************************************


                                    regctrl

2009/04  morimoto
************************************************************************/
#include "dmm.h"
#include "regs.h"

static u32 s_tblRegs[REGMAX];
static const char*   s_strRegID[REGMAX];  // id <--> namex

//======================================================================
//
//
//                      local function
//
//
//======================================================================
//=====================================
//
//          getregshift
//
//=====================================
static int getregshift( STREGCTRL *pRegCtrl,
                        int        nPos )
{
    STMEMCTRL *mc     = NULL;
    STREG     *member = NULL;
    int shift = 0;
    int i     = 0;

    if ( !pRegCtrl )
        return -1;

    mc = GetMemCtrl( pRegCtrl->EType );
    if ( !mc )
        return -1;

    for ( member = pRegCtrl->pMember , i = 0;
          member->pstrName;
          member++ , i++ ) {
        shift += member->nSize;
        if ( nPos == i )
            break;
    }

    if ( nPos != i )
        return -1;

    return (int)( 8 * mc->nIncSize ) - shift;
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
//          _sysRegInit
//
//=====================================
void _sysRegInit( void )
{
    int i = 0;

    if ( GetCPU( ) )
        return;

    for ( i=REG_START ; i<REG_END ; i++ )
        s_tblRegs[i] = REGUNDEF;

#define DEF(a,b) s_strRegID[a] = b;
#include "regs.def"
#undef DEF

}

//=====================================
//
//          RegSyncRead
//
//=====================================
bool RegSyncRead( STREGCTRL *pRegCtrl )
{
    STMEMCTRL *memc = GetMemCtrl( pRegCtrl->EType );
    u32        addr;

    addr = GetRegAddr( pRegCtrl->nID );

    return memc->fnRead( addr, &pRegCtrl->ccVal );
}

//=====================================
//
//          RegSyncWrite
//
//=====================================
bool RegSyncWrite( STREGCTRL *pRegCtrl )
{
    STMEMCTRL *memc = GetMemCtrl( pRegCtrl->EType );
    u32        addr;

    addr = GetRegAddr( pRegCtrl->nID );

    return memc->fnEdit( addr, &pRegCtrl->ccVal );
}

//=====================================
//
//          GetRegVal
//
// RegSyncRead is needed before call this function
//=====================================
u32 GetRegVal( STREGCTRL *pRegCtrl,
               int        nPos )
{
    u32 val = pRegCtrl->ccVal;
    u32 mask;
    int shift = getregshift( pRegCtrl, nPos );

    if ( shift < 0 )
        return 0;

    mask = (u32)( 1 << pRegCtrl->pMember[nPos].nSize ) - 1;
    val  = val >> shift;

    return val & mask;
}

//=====================================
//
//          SetRegVal
//
// RegSyncWrite is needed before call this function
//=====================================
u32 SetRegVal( STREGCTRL *pRegCtrl,
               int        nPos,
               u32        ulSet )
{
    u32 val = pRegCtrl->ccVal;
    u32 mask;
    int shift = getregshift( pRegCtrl, nPos );

    if ( shift < 0 )
        return 0;

    mask  = (u32)( 1 << pRegCtrl->pMember[nPos].nSize ) - 1;
    ulSet  &= mask;

    val  &= ~(mask << shift);
    val  |=  (ulSet  << shift);

    return val;
}

//=====================================
//
//          GetRegExplain
//
// RegSyncRead is needed before call this function
//=====================================
const char *GetRegExplain( STREGCTRL *pRegCtrl,
                           int nPos)
{
    u32 i;
    u32 val = GetRegVal( pRegCtrl , nPos );
    STREG *member = NULL;

    // get position
    for ( member = pRegCtrl->pMember, i=0 ;
          member->pstrName;
          member++, i++ ) {
        if ( nPos == i )
            break;
    }

    if ( nPos != i )
        return "** regctrl doesn't have name **";

    for ( i=0 ; i<=val ; i++ ) {
        if ( !member->ppExplain[i] )
            return "** regctrl doesn't have explain **";
    }

    return member->ppExplain[val];
}

//=====================================
//
//          GetRegID
//
//=====================================
REGID GetRegID( const char *pstrInput,
                REGID nStart ,
                REGID nEnd )
{
    int i;

    if ( nStart > nEnd ){
        Error( "param error" );
        return REG_END;
    }
    if ( nEnd > REG_END ) {
        Error( "reg size error" );
        return REG_END;
    }

    for ( i=nStart ; i<nEnd ; i++ ) {
        if ( 0 == strcmp( s_strRegID[i] , pstrInput ))
            return i;
    }

    return REG_END;
}

//=====================================
//
//          GetRegCmdName
//
//=====================================
const char* GetRegCmdName( REGID nID )
{
    if ( nID <  REG_START ||
         nID >= REG_END ) {
        Error( "reg size error" );
        return NULL;
    }

    return s_strRegID[nID];
}

//=====================================
//
//          GetRegName
//
//=====================================
const char* GetRegName( REGID nID )
{
    const char* pname = "None";
    switch ( nID ) {
#define DEF(a,b) case a: pname = #a; break;
#include "regs.def"
#undef DEF
    default:
        Error( "register is not defined" );
    }
    return pname;
}

//=====================================
//
//          GetRegAddr
//
//=====================================
u32 GetRegAddr( REGID nID )
{
    return s_tblRegs[ nID ];
}

//=====================================
//
//          RegCheck
//
//=====================================
bool RegCheck( REGID nID )
{
    if ( REGUNDEF == s_tblRegs[ nID ] )
        return false;

    return true;
}

//=====================================
//
//          SetRegs
//
//=====================================
void SetRegs( int ixName , u32 Param )
{
    if ( 0       > ixName  ||
         REGMAX <= ixName ) {
        Error( "regs size error" );
        return;
    }

    s_tblRegs[ixName] = Param;
}
