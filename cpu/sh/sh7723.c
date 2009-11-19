/*=============================================

                     sh7723

2008/09  morimoto
==============================================*/
#include "dmm.h"
#include "cpu/sh/regs.h"
#include "cmd/cmdlist.h"
#include "cmd/cmd-sh-pfc-v.h"

static STPFCCTRL pxcr[] = {

    //              7                 6                 5                4
    //              3                 2                 1                0
    SETPFC( PACR,   PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),
                    PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 )),
    SETPFC( PBCR,   PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),
                    PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 )),
    SETPFC( PCCR,   PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),
                    PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 )),
    SETPFC( PDCR,   PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),
                    PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 )),
    SETPFC( PECR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),
                    PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 )),
    SETPFC( PFCR,   PFC_FN( "ATAPI" ),PFC_FNS( 0, 4 ),  PFC_FNS( 0, 4 ),  PFC_FNS( 0, 2 ),
                    PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 )),
    SETPFC( PGCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" ),
                    PFC_FNS( 0, 5 ),  PFC_FNS( 0, 5 ),  PFC_FNS( 0, 5 ),  PFC_FNS( 0, 5 )),
    SETPFC( PHCR,   PFC_FN( "LCDC" ), PFC_FNS( 1, 0 ),  PFC_FNS( 1, 0 ),  PFC_FNS( 1, 1 ),
                    PFC_FNS( 1, 1 ),  PFC_FN( "LCDC" ), PFC_FNS( 1, 1 ),  PFC_FN( "LCDC" )),
    SETPFC( PJCR,   PFC_FN( "Sys" ),  PFC_FN( "***" ),  PFC_FN( "Sys" ),  PFC_FN( "***" ),
                    PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" )),
    SETPFC( PKCR,   PFC_FN( "SIU" ),  PFC_FNS( 1, 3 ),  PFC_FNS( 1, 3 ),  PFC_FNS( 1, 4 ),
                    PFC_FNS( 1, 4 ),  PFC_FNS( 1, 4 ),  PFC_FNS( 1, 4 ),  PFC_FNS( 1, 4 )),
    SETPFC( PLCR,   PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),
                    PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 )),
    SETPFC( PMCR,   PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),
                    PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 )),
    SETPFC( PNCR,   PFC_FNS( 1, 6 ),  PFC_FNS( 1, 6 ),  PFC_FNS( 1, 6 ),  PFC_FNS( 1, 6 ),
                    PFC_FNS( 1, 6 ),  PFC_FNS( 1, 6 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 5 )),
    SETPFC( PQCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),
                    PFC_FN( "ADC" ),  PFC_FN( "ADC" ),  PFC_FN( "ADC" ),  PFC_FN( "ADC" )),
    SETPFC( PRCR,   PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),
                    PFC_FNS( 0, 1 ),  PFC_FN( "BSC" ),  PFC_FN( "BSC"),   PFC_FN( "BSC" )),
    SETPFC( PSCR,   PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),
                    PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 )),
    SETPFC( PTCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 2, 1 ),  PFC_FNS( 2, 1 ),
                    PFC_FNS( 2, 1 ),  PFC_FNS( 2, 2 ),  PFC_FNS( 2, 2 ),  PFC_FNS( 2, 2 )),
    SETPFC( PUCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 2, 3 ),  PFC_FNS( 2, 3 ),
                    PFC_FNS( 2, 3 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 )),
    SETPFC( PVCR,   PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),
                    PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 4 )),
    SETPFC( PWCR,   PFC_FN( "INTC" ), PFC_FN( "INTC" ), PFC_FN( "INTC" ), PFC_FNS( 3, 0 ),
                    PFC_FNS( 3, 1 ),  PFC_FNS( 3, 2 ),  PFC_FNS( 3, 3 ),  PFC_FNS( 3, 4 )),
    SETPFC( PXCR,   PFC_FN( "DMAC" ), PFC_FNS( 3, 6 ),  PFC_FNS( 3, 5 ),  PFC_FNS( 3, 5 ),
                    PFC_FN( "TSIF" ), PFC_FN( "TSIF" ), PFC_FN( "TSIF" ), PFC_FN( "TSIF" )),
    SETPFC( PYCR,   PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),
                    PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" )),
    SETPFC( PZCR,   PFC_FN( "SIU" ),  PFC_FN( "SIU" ),  PFC_FN( "SIU" ),  PFC_FN( "SIU" ),
                    PFC_FNS( 3, 7 ),  PFC_FNS( 3, 7 ),  PFC_FNS( 3, 7 ),  PFC_FNS( 3, 7 )),
};

static STREGCTRL psel[] = {
    SETREG( PSELA, TWORD,
            EREG( "15-14", 2, "BSC", "KEYSC" ),
            EREG( "13-12", 2, "BSC", "LCDC" ),
            EREG( "11-10", 2, "ATAPI", "SDHI/SCIF/MSIOF" ),
            DREG( "9-6",   4 ),
            EREG( "5-4",   2, "ATAPI", "MSIOF(1)" , "MSIOF(2)" ),
            EREG( "3-2",   2, "AUD", "TPU" ),
            DREG( "1-0",   2 )),
    SETREG( PSELB, TWORD,
            EREG( "15-14", 2, "LCDC", "VOU" ),
            EREG( "13-12", 2, "LCDC(RGB)", "LCDC(SYS)" ),
            DREG( "11-10", 2 ),
            EREG( "9-8",   2, "SIU", "MSIOF(1)", "MSIOF(2)" ),
            EREG( "7-6",   2, "SIU", "MSIOF" ),
            EREG( "5-4",   2, "LCDC", "VOU" ),
            EREG( "3-2",   2, "LCDC", "SCIF" ),
            DREG( "1-0",   2 )),
    SETREG( PSELC, TWORD,
            EREG( "15-14",  2, "SCIF", "SDHI" ),
            EREG( "13-12",  2, "SCIF", "MSIOF" ),
            EREG( "11-10",  2, "SCIF", "MSIOF(1)", "MSIOF(2)" ),
            EREG( "9-8",    2, "FLCTL", "SCIF" ),
            EREG( "7-6",    2, "FLCTL", "SCIF", "VOI" ),
            DREG( "5-0",    6 )),
    SETREG( PSELD, TWORD,
            EREG( "15-14",  2, "INTC", "LCDC" ),
            EREG( "13-12",  2, "INTC", "ADC" ),
            EREG( "11-10",  2, "INTC", "BSC", "VIO" ),
            EREG( "9-8",    2, "INTC", "SIU" ),
            EREG( "7-6",    2, "INTC", "SIU" ),
            EREG( "5-4",    2, "DMAC","IrDA" ),
            EREG( "3-2",    2, "DMAC", "MSIOF" ),
            EREG( "1-0",    2, "VIO", "SIU" )),
};

static STSHPFCCTRL pfc_ctrl = {
    .pPxCR      = pxcr,
    .nPxCRSize  = ARRAY_SIZE( pxcr ),
    .pPSEL      = psel,
    .nPSELSize  = ARRAY_SIZE( psel ),
};

static void cmdinit( const STCMD *pCmd )
{
    if ( pCmd == &cmd_pfc ) {

        SetCmdData( "pfc" , (void*)&pfc_ctrl );
        SetRegs( PACR    , 0xA4050100 );
        SetRegs( PBCR    , 0xA4050102 );
        SetRegs( PCCR    , 0xA4050104 );
        SetRegs( PDCR    , 0xA4050106 );
        SetRegs( PECR    , 0xA4050108 );
        SetRegs( PFCR    , 0xA405010A );
        SetRegs( PGCR    , 0xA405010C );
        SetRegs( PHCR    , 0xA405010E );
        SetRegs( PJCR    , 0xA4050110 );
        SetRegs( PKCR    , 0xA4050112 );
        SetRegs( PLCR    , 0xA4050114 );
        SetRegs( PMCR    , 0xA4050116 );
        SetRegs( PNCR    , 0xA4050118 );
        SetRegs( PQCR    , 0xA405011A );
        SetRegs( PRCR    , 0xA405011C );
        SetRegs( PSCR    , 0xA405011E );
        SetRegs( PTCR    , 0xA4050140 );
        SetRegs( PUCR    , 0xA4050142 );
        SetRegs( PVCR    , 0xA4050144 );
        SetRegs( PWCR    , 0xA4050146 );
        SetRegs( PXCR    , 0xA4050148 );
        SetRegs( PYCR    , 0xA405014A );
        SetRegs( PZCR    , 0xA405014C );
        SetRegs( PSELA   , 0xA405014E );
        SetRegs( PSELB   , 0xA4050150 );
        SetRegs( PSELC   , 0xA4050152 );
        SetRegs( PSELD   , 0xA4050154 );
        SetRegs( HIZCRA  , 0xA4050158 );
        SetRegs( HIZCRB  , 0xA405015A );
        SetRegs( HIZCRC  , 0xA405015C );
        SetRegs( HIZCRD  , 0xA405015E );
        SetRegs( MSELCRA , 0xA4050180 );
        SetRegs( MSELCRB , 0xA4050182 );
        SetRegs( PULCR   , 0xA4050184 );
        SetRegs( DRVCRA  , 0xA405018A );
        SetRegs( DRVCRB  , 0xA405018C );
    }
    else if ( pCmd == &cmd_io ) {
        SetRegs( PADR    , 0xA4050120 );
        SetRegs( PBDR    , 0xA4050122 );
        SetRegs( PCDR    , 0xA4050124 );
        SetRegs( PDDR    , 0xA4050126 );
        SetRegs( PEDR    , 0xA4050128 );
        SetRegs( PFDR    , 0xA405012A );
        SetRegs( PGDR    , 0xA405012C );
        SetRegs( PHDR    , 0xA405012E );
        SetRegs( PJDR    , 0xA4050130 );
        SetRegs( PKDR    , 0xA4050132 );
        SetRegs( PLDR    , 0xA4050134 );
        SetRegs( PMDR    , 0xA4050136 );
        SetRegs( PNDR    , 0xA4050138 );
        SetRegs( PQDR    , 0xA405013A );
        SetRegs( PRDR    , 0xA405013C );
        SetRegs( PSDR    , 0xA405013E );
        SetRegs( PTDR    , 0xA4050160 );
        SetRegs( PUDR    , 0xA4050162 );
        SetRegs( PVDR    , 0xA4050164 );
        SetRegs( PWDR    , 0xA4050166 );
        SetRegs( PXDR    , 0xA4050168 );
        SetRegs( PYDR    , 0xA405016A );
        SetRegs( PZDR    , 0xA405016C );
    }
    else if ( pCmd == &cmd_mstp ) {
        SetRegs( STBCR   , 0xA4150020 );
        SetRegs( MSTPCR0 , 0xA4150030 );
        SetRegs( MSTPCR1 , 0xA4150034 );
        SetRegs( MSTPCR2 , 0xA4150038 );
    }
    else if ( pCmd == &cmd_bsc ) {
        SetRegs( CMNCR   , 0xFEC10000 );
        SetRegs( CS0BCR  , 0xFEC10004 );
        SetRegs( CS4BCR  , 0xFEC10010 );
        SetRegs( CS5ABCR , 0xFEC10014 );
        SetRegs( CS5BBCR , 0xFEC10018 );
        SetRegs( CS6ABCR , 0xFEC1001C );
        SetRegs( CS6BBCR , 0xFEC10020 );
        SetRegs( CS0WCR  , 0xFEC10024 );
        SetRegs( CS4WCR  , 0xFEC10030 );
        SetRegs( CS5AWCR , 0xFEC10034 );
        SetRegs( CS5BWCR , 0xFEC10038 );
        SetRegs( CS6AWCR , 0xFEC1003C );
        SetRegs( CS6BWCR , 0xFEC10040 );
    }
}

static void init( const STCPU *pCpu )
{
    EnableCmd( "pfc" );
    EnableCmd( "io" );
    EnableCmd( "mstp" );
    EnableCmd( "bsc" );
}

INSTALL_CPU( SH7723 )
