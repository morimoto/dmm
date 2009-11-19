/************************************************************************


                      pfc ( if cpu is sh )


************************************************************************/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"
#include "cmd/cmd-sh-pfc-v.h"

static STHELPMSG s_Help[] = {
    {
        "pfc"  , "dump, edit PFC register" ,
        "\n"
        "dmm pfc [rwsmh] [param]\n\n"
        "[option]          [param]\n"
        " -r : read         a-z  :  PACR - PZCR\n"
        " -w : write        sa-se : PSELA - PSELE\n"
        " -s : set bit      ha-hd : HIZCRA - HIZCRD\n"
        " -c : clear bit    ma-mb : MSELCRA - MSELCRB\n"
        " -h : print help   pu    : PULCR\n"
        "                   sbscr : SBSCR\n"
        "                   drcr  : DRVCR\n"
        "                   da-db : DRVCRA - DRVCRB\n"
        " -v : print detail"
        "\n"
        "ex) case sa is 0x1234\n"
        "dmm pfc sa            => 0x1234\n"
        "dmm pfc -r sa         => 0x1234\n"
        "dmm pfc -w sa 0x5678  => 0x5678\n"
        "dmm pfc -s sa 0x8001  => 0x9235\n"
        "dmm pfc -c sa 0x000f  => 0x1230\n"
        "dmm pfc -v a b c\n"
        "\n"
    },
    HELPMSGEND
};

//=====================================
//
//          print_bit
//
//=====================================
static void print_bit( u32 Val, int nSize )
{
    u32 mask = (u32)1 << ( nSize - 1 );
    int i;

    for ( i=0 ; i<nSize ; i++ ) {
        printf( "%d", ( Val & mask ) ? 1 : 0 );
        mask = mask >> 1;
    }
}

//=====================================
//
//               pfc -v
//
//=====================================
extern const STCMD cmd_pfc;
static bool pfc_v( REGID nID )
{

    STMEMCTRL   *pmctrl = NULL;
    STSHPFCCTRL *pfc_ctrl = (STSHPFCCTRL*)GetCmdData( "pfc" );
    bool ret = false;
    int i;
    u32 val;
    STREG *p;
    STREGCTRL *psel = pfc_ctrl->pPSEL;
    STPFCCTRL *pxcr_ctrl = NULL;
    STREGCTRL  pxcr_val = SETREG( 0, TWORD, // dummy reg
                                  DREG( "px7md", 2 ),
                                  DREG( "px6md", 2 ),
                                  DREG( "px5md", 2 ),
                                  DREG( "px4md", 2 ),
                                  DREG( "px3md", 2 ),
                                  DREG( "px2md", 2 ),
                                  DREG( "px1md", 2 ),
                                  DREG( "px0md", 2 ));

    pmctrl = GetMemCtrl( pxcr_val.EType );
    if ( !pmctrl )
        return Error( "get mem ctrl error" );

    for ( i=0 ; i<pfc_ctrl->nPxCRSize ; i++ )
        if ( nID == pfc_ctrl->pPxCR[i].nID ) {
            pxcr_ctrl = pfc_ctrl->pPxCR + i;
            break;
        }

    if ( !pxcr_ctrl )
        return Error( "Unknown ID" );

    printf( "-------------------------\n" );

    //----------------------
    // get PxnMD value from PxCR register
    //----------------------
    pmctrl->fnRead( GetRegAddr( nID ), &pxcr_val.ccVal );
    BLUE;
    printf( "%-8s (" , GetRegName( nID ));
    CLAR;
    pmctrl->fnDump( GetRegAddr( nID ));
    printf( ")\n" );

    //----------------------
    // get PSEL value from PSELx register
    //----------------------
    for ( i=0 ; i<pfc_ctrl->nPSELSize ; i++ ) {
        RegSyncRead( psel + i );
        printf( "%-8s (" , GetRegName( psel[i].nID ));
        pmctrl->fnDump( GetRegAddr( psel[i].nID ));
        printf( ")\n" );
    }

    printf( "\n" );

    for ( p = pxcr_val.pMember, i=0;
          p->pstrName ;
          p++ , i++ ) {

        val = GetRegVal( &pxcr_val, i );

        printf( "%s " , p->pstrName );
        //----------------------
        // if val is not 0x00
        // It mean I/O
        //----------------------
        if ( val ) {
            printf( "%8s (" , "I/O");
            print_bit( val, p->nSize );
            printf( ")\n" );
        }
        //----------------------
        // if val is 0x00
        // It mean Function
        //----------------------
        else {

            if ( pxcr_ctrl->pMember[i].pFncName ) {
                printf( "%8s (" , pxcr_ctrl->pMember[i].pFncName );
                print_bit( val, p->nSize );
                printf( ")\n" );
            }
            else {
                int   reg_pos = pxcr_ctrl->pMember[i].nRegPos;
                int   val_pos = pxcr_ctrl->pMember[i].nValPos;
                STREGCTRL *ps = pfc_ctrl->pPSEL + reg_pos;

                val = GetRegVal( ps , val_pos );
                printf( "%8s (", GetRegExplain( ps , val_pos ));
                print_bit( val, p->nSize );
                printf( ") # %s (%s)\n" ,
                        GetRegName( ps->nID ),
                        (ps->pMember + val_pos)->pstrName );
            }

        }
    }
    ret = true;

    return ret;

}

//=====================================
//
//               cmd
//
//=====================================
static bool cmd( int nArgc, char *pstrArgv[] )
{
    //----------------------
    //
    // for -v option
    //
    // pstrArgv[0]  => command name
    // pstrArgv[1]  => 1st option
    //
    //----------------------
    if ( nArgc > 1 &&
         0 == strcmp( "-v", pstrArgv[1] )) {

        int i;
        REGID id;
        bool ret;

        if ( !GetCmdData( "pfc" ) )
            return Error( "This CPU doesn't support pfc -v" );

        // remove func name , option
        nArgc    -= 2;
        pstrArgv += 2;

        if ( nArgc < 1 )
            return Error( "you have to set ID" );

        if( !MemInit(  ))
            return Error( "mem ctrl init error" );

        for ( i=0 ; i<nArgc ; i++ ) {

            id = GetRegID( pstrArgv[i] , PACR , PZCR + 1 );
            ret = false;
            if (( PACR > id ) || ( PZCR + 1 <= id )) {
                Error( "ID error" );
                break;
            }

            if ( !RegCheck( id ))
                break;

            if ( !pfc_v( id ))
                break;

            ret = true;
        }

        MemExit(  );

        return ret;
    }

    return CmdCommon( nArgc , pstrArgv , TWORD , PFC_START , PFC_END );

}

//=====================================
//
//          ishit
//
//=====================================
static bool
ishit( int nArgc, char *pstrArgv[] )
{
    if ( IsCmdHit( ))
        return true;

    return false;
}
INSTALL_CMD( pfc )
