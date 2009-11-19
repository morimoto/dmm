/************************************************************************


                                  cmd

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include "cmdlist.c"

//======================================================================
//
//
//                      local function
//
//
//======================================================================
//=====================================
//
//          print_reg
//
//=====================================
static bool print_reg( REGID nID , STMEMCTRL *pMctrl )
{
    u32 addr = 0;

    addr = GetRegAddr( nID );

    BLUE;
    printf( "%-8s (0x%08lx) : " , GetRegName( nID ) , addr );
    CLAR;

    pMctrl->fnDump( addr );
    printf( "\n" );

    return true;
}

//=====================================
//
//          write_reg
//
//=====================================
static bool write_reg( STMEMCTRL *pMctrl , u32 tAddr , const char* pName , int nArgc, char *pstrArgv[] )
{
    u32 data = GetData( pstrArgv[0] );

    if ( 1 != nArgc )
        return false;

    printf( "write %s (0x%08lx)\n" , pName , tAddr );
    printf( "before : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;

    printf( "\n" );
    if( !pMctrl->fnEdit( tAddr , &data ))
        return false;

    printf( "after  : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;

    printf( "\n" );

    return true;
}

//=====================================
//
//          setbit_reg
//
//=====================================
static bool setbit_reg( STMEMCTRL *pMctrl , u32 tAddr , const char* pName , int nArgc, char *pstrArgv[] )
{
    u32 data = 0;

    if ( 1 != nArgc )
        return false;

    printf( "write %s (0x%08lx)\n" , pName , tAddr );
    printf( "before : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;

    printf( "\n" );
    if( !pMctrl->fnRead( tAddr , &data ))
        return false;

    data |= GetData( pstrArgv[0] );

    if( !pMctrl->fnEdit( tAddr , &data ))
        return false;

    printf( "after  : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;
    printf( "\n" );

    return true;
}

//=====================================
//
//          clearbit_reg
//
//=====================================
static bool clearbit_reg( STMEMCTRL *pMctrl , u32 tAddr , const char* pName , int nArgc , char *pstrArgv[] )
{
    u32 data = 0;

    if ( 1 != nArgc )
        return false;

    printf( "write %s (0x%08lx)\n" , pName , tAddr );
    printf( "before : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;

    printf( "\n" );
    if( !pMctrl->fnRead( tAddr , &data ))
        return false;

    data &= ~GetData( pstrArgv[0] );

    if( !pMctrl->fnEdit( tAddr , &data ))
        return false;

    printf( "after  : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;
    printf( "\n" );

    return true;
}

//=====================================
//
//          maskset_reg
//
//=====================================
static bool maskset_reg( STMEMCTRL *pMctrl , u32 tAddr , const char* pName , int nArgc , char *pstrArgv[] )
{
    u32 data = 0;
    u32 mask = GetData( pstrArgv[0] );

    if ( 2 != nArgc )
        return false;

    printf( "write %s (0x%08lx)\n" , pName , tAddr );
    printf( "before : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;

    printf( "\n" );
    if( !pMctrl->fnRead( tAddr , &data ))
        return false;

    data = (data & ~mask) | (GetData( pstrArgv[1] ) & mask);

    if( !pMctrl->fnEdit( tAddr , &data ))
        return false;

    printf( "after  : ");
    if( !pMctrl->fnDump( tAddr ))
        return false;
    printf( "\n" );

    return true;
}

//=====================================
//
//          cmdtemp
//
// pstrArgv 0 id
//          1 data
//=====================================
#define ARGMAX 3
static bool cmdtemp( int nArgc, char *pstrArgv[] , int arg_max,
                     enum ESTYPE EStype , REGID idStart , REGID idEnd ,
                     bool (*pfnFunc)( STMEMCTRL* , u32 , const char* , int, char ** ))
{
    bool           ret    = false;
    REGID          id     = idStart;
    STMEMCTRL     *pmctrl = NULL;
    u32            addr   = 0;

    if ( arg_max != nArgc )
        return false;

    arg_max--;

    if ( arg_max >= ARGMAX ) {
        printf( "program error\nneed fix\n" );
        return false;
    }

    if( !MemInit(  ))
        return Error( "mem ctrl init error" );

    pmctrl = GetMemCtrl( EStype );
    if ( !pmctrl ) {
        Error( "get mem ctrl error" );
        goto error;
    }

    //----------------------
    // write selected
    //----------------------
    id = GetRegID( pstrArgv[0] , idStart , idEnd );
    if (( idStart >  id ) || ( idEnd <= id ))
        goto error;

    if ( !RegCheck( id )) {
        printf( "This CPU don't have %s\n" , GetRegName( id ));
        goto error;
    }

    addr = GetRegAddr( id );

    if ( !(*pfnFunc)( pmctrl , addr , GetRegName( id ) , arg_max , pstrArgv + 1 ))
        goto error;

    ret = true;

 error:
    MemExit(  );

    return ret;
}

//=====================================
//
//          cmdread
//
//=====================================
static bool cmdread( int nArgc, char *pstrArgv[] ,
                     enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    bool           ret    = false;
    REGID          id     = idStart;
    STMEMCTRL     *pmctrl = NULL;

    if( !MemInit(  ))
        return Error( "mem ctrl init error" );

    pmctrl = GetMemCtrl( EStype );
    if ( !pmctrl ) {
        Error( "get mem ctrl error" );
        goto error;
    }

    //----------------------
    // print all
    //----------------------
    if ( 0 == nArgc ) {
        for( id = idStart; id < idEnd ; id++ ){
            if ( !RegCheck( id ))
                continue;

            if ( !print_reg( id , pmctrl ) )
                goto error;
        }
    }
    //----------------------
    // print selected
    //----------------------
    else {
        int i = 0;
        for ( i=0 ; i<nArgc ; i++ ) {
            id = GetRegID( pstrArgv[i] , idStart , idEnd );
            if (( idStart >  id ) || ( idEnd <= id )) 
                goto error;

            if ( !RegCheck( id )) {
                printf( "This CPU don't have %s\n" , GetRegName( id ));
                ret = true;
                goto error;
            }
            if ( !print_reg( id , pmctrl ))
                goto error;

        }
    }
    ret = true;

 error:
    MemExit(  );

    return ret;
}

//=====================================
//
//          cmdwrite
//
//=====================================
static bool cmdwrite( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    return cmdtemp( nArgc, pstrArgv , 2, EStype , idStart , idEnd , write_reg );
}

//=====================================
//
//          cmdsetbit
//
//=====================================
static bool cmdsetbit( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    return cmdtemp( nArgc, pstrArgv , 2, EStype , idStart , idEnd , setbit_reg );
}

//=====================================
//
//          cmdclearbit
//
//=====================================
static bool cmdclearbit( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    return cmdtemp( nArgc, pstrArgv , 2, EStype , idStart , idEnd , clearbit_reg );
}

//=====================================
//
//          cmdmaskset
//
//=====================================
static bool cmdmaskset( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    return cmdtemp( nArgc, pstrArgv , 3, EStype , idStart , idEnd , maskset_reg );
}

//=====================================
//
//          cmdhelp
//
//=====================================
static bool cmdhelp( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    int          i, cnt = 0;

    printf( "\n[supported regs]\n\n" );
    for ( i=idStart ; i<idEnd ; i++ ) {
        if ( !RegCheck( i ))
            continue;
        printf( "%s, " , GetRegCmdName( i ) );
        if ( 9 == cnt%10 )
            printf( "\n" );
        cnt++;
    }
    printf( "\n" );

    return true;
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
//               Usage
//
//=====================================
bool Usage( const char *pMsg )
{
    int        i     = 0;
    STHELPMSG *phelp = NULL;

    printf( "[ %s ]\n" , pMsg );

    // print --XXX option
    for ( i=0 ; i<s_nCmdListSize ; i++ ){
        if( !s_tblCmdList[i].isEnable )
            continue;

        phelp = s_tblCmdList[i].pCmd->pHelp;
        while ( phelp->pstrCmd ) {
            if (( 2   < strlen( phelp->pstrCmd )) &&
                ('-' == phelp->pstrCmd[0]      )  &&
                ('-' == phelp->pstrCmd[1]      ))
                printf( "%-15s : %s\n" , phelp->pstrCmd , phelp->pstrUsage );

            phelp++;
        }
    }

    // print other help
    for ( i=0 ; i<s_nCmdListSize ; i++ ){
        if( !s_tblCmdList[i].isEnable )
            continue;

        phelp = s_tblCmdList[i].pCmd->pHelp;
        while ( phelp->pstrCmd ) {
            if ('-' != phelp->pstrCmd[0] )
                printf( "%-15s : %s\n" , phelp->pstrCmd , phelp->pstrUsage );

            phelp++;
        }
    }

    printf( "\n"
            PRODUCE_BY "\n"
            SPECIAL_THANKS "\n"
            );

    return true;
}

//=====================================
//
//          IsCmdHit
//
//=====================================
bool _IsCmdHit( const char *pstrCmd , STHELPMSG *pHelp )
{
    while ( pHelp->pstrCmd ) {
        if ( CMD_HIT( pHelp->pstrCmd , pstrCmd ))
            return true;
        pHelp++;
    }
    return false;
}


//=====================================
//
//          CmdGetDataSize
//
// copy from uboot
//=====================================
enum ESTYPE CmdGetDataSize( char* pstrArg, enum ESTYPE EDefaultType )
{
    // Check for a size specification .b, .w or .l.
    size_t len = strlen( pstrArg );

    if ( len > 2 && pstrArg[ len-2 ] == '.' ) {
        switch( pstrArg[ len-1 ] ) {
        case 'b': return TBYTE;
        case 'w': return TWORD;
        case 'l': return TLONG;
        default:  return TERR;
        }
    }
    return EDefaultType;
}

//=====================================
//
//          CmdCommon
//
//=====================================
bool CmdCommon( int nArgc, char *pstrArgv[] , enum ESTYPE EStype , REGID idStart , REGID idEnd )
{
    bool (*cmd)( int , char ** , enum ESTYPE , REGID , REGID );

    // pstrArgv[0]  => command name
    // pstrArgv[1]  => 1st option
    // ...

    if ( 1 > nArgc  )
        return false;

         if ( 1 == nArgc )                        cmd = cmdread;
    else if ( 0 == strcmp( "-r" , pstrArgv[1] )){ cmd = cmdread;    nArgc--; pstrArgv++; }
    else if ( 0 == strcmp( "-w" , pstrArgv[1] )){ cmd = cmdwrite;   nArgc--; pstrArgv++; }
    else if ( 0 == strcmp( "-s" , pstrArgv[1] )){ cmd = cmdsetbit;  nArgc--; pstrArgv++; }
    else if ( 0 == strcmp( "-c" , pstrArgv[1] )){ cmd = cmdclearbit;nArgc--; pstrArgv++; }
    else if ( 0 == strcmp( "-m" , pstrArgv[1] )){ cmd = cmdmaskset; nArgc--; pstrArgv++; }
    else if ( 0 == strcmp( "-h" , pstrArgv[1] )){ cmd = cmdhelp;    nArgc--; pstrArgv++; }
    else                                          cmd = cmdread;

    // remove func name
    nArgc--;
    pstrArgv++;

    if ( 0 > nArgc )
        return false;

    return (*cmd)( nArgc , pstrArgv , EStype , idStart , idEnd );

}

//=====================================
//
//          GetCmdListSize
//
//=====================================
int GetCmdListSize( void )
{
    return s_nCmdListSize;
}

//=====================================
//
//          GetCmdList
//
//=====================================
const STCMD* GetCmdList( int ixCmd )
{
    if ( 0 > ixCmd ||
         s_nCmdListSize <= ixCmd ) {
        Error( "Cmd List index error" );
        return NULL;
    }

    return s_tblCmdList[ixCmd].pCmd;
}

//=====================================
//
//          EnableCmd
//
//=====================================
void EnableCmd( const char* pstrName )
{
    int i;

    for ( i=0 ; i<s_nCmdListSize ; i++ ) {
        if ( 0 == strcmp( pstrName,
                          s_tblCmdList[i].pCmd->pstrName )) {
            s_tblCmdList[i].isEnable = true;
            break;
        }
    }
}

//=====================================
//
//          IsEnableCmd
//
//=====================================
bool IsEnableCmd( int ixCmd )
{
    if ( 0 > ixCmd ||
         s_nCmdListSize <= ixCmd ) {
        return false;
    }

    return s_tblCmdList[ixCmd].isEnable;
}

//=====================================
//
//          SetCmdData
//
//=====================================
void SetCmdData( const char* pstrName,
                 void* pData )
{
    int i;

    for ( i=0 ; i<s_nCmdListSize ; i++ ) {
        if ( 0 == strcmp( pstrName,
                          s_tblCmdList[i].pCmd->pstrName )) {
            s_tblCmdList[i].pPrivateData = pData;
            break;
        }
    }
}

//=====================================
//
//          GetCmdData
//
//=====================================
void* GetCmdData( const char* pstrName )
{
    int i;

    for ( i=0 ; i<s_nCmdListSize ; i++ ) {
        if ( 0 == strcmp( pstrName,
                          s_tblCmdList[i].pCmd->pstrName )) {
            return s_tblCmdList[i].pPrivateData;
        }
    }
    return NULL;
}
