/*=============================================

                     sh7724

2009/03  morimoto
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
    SETPFC( PBCR,   PFC_FNS( 4, 0 ),  PFC_FNS( 4, 0 ),  PFC_FNS( 4, 2 ),  PFC_FNS( 4, 2 ),
                    PFC_FNS( 4, 2 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 ),  PFC_FNS( 0, 0 )),
    SETPFC( PCCR,   PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ),
                    PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" )),
    SETPFC( PDCR,   PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ),
                    PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" )),
    SETPFC( PECR,   PFC_FN( "FSI" ),  PFC_FN( "FSI" ),  PFC_FNS( 2, 5 ),  PFC_FNS( 3, 8 ),
                    PFC_FNS( 0, 11 ), PFC_FNS( 0, 11 ), PFC_FN( "LCDC" ), PFC_FN( "LCDC" )),
    SETPFC( PFCR,   PFC_FN( "LCDC" ), PFC_FNS( 3, 3 ),  PFC_FNS( 3, 3 ),  PFC_FN( "LCDC" ),
                    PFC_FNS( 3, 3 ),  PFC_FNS( 0, 8 ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 5 )),
    SETPFC( PGCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" ),
                    PFC_FN( "AUD" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" )),
    SETPFC( PHCR,   PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),
                    PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" )),
    SETPFC( PJCR,   PFC_FN( "Sys" ),  PFC_FN( "Sys" ),  PFC_FN( "Sys" ),  PFC_FN( "***" ),
                    PFC_FNS( 0, 6 ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" )),
    SETPFC( PKCR,   PFC_FNS( 1, 7 ),  PFC_FNS( 1, 7 ),  PFC_FNS( 1, 7 ),  PFC_FNS( 1, 7 ),
                    PFC_FNS( 1, 7 ),  PFC_FNS( 1, 7 ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" )),
    SETPFC( PLCR,   PFC_FNS( 1, 6 ),  PFC_FNS( 1, 6 ),  PFC_FNS( 4, 5 ),  PFC_FNS( 2, 3 ),
                    PFC_FNS( 2, 3 ),  PFC_FNS( 2, 3 ),  PFC_FNS( 3, 7 ),  PFC_FNS( 4, 6 )),
    SETPFC( PMCR,   PFC_FNS( 2, 1 ),  PFC_FNS( 2, 1 ),  PFC_FNS( 2, 1 ),  PFC_FNS( 2, 1 ),
                    PFC_FNS( 2, 2 ),  PFC_FNS( 2, 2 ),  PFC_FNS( 0, 8 ),  PFC_FNS( 0, 7 )),
    SETPFC( PNCR,   PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VOU" ),  PFC_FNS( 3, 1 ),
                    PFC_FNS( 3, 0 ),  PFC_FNS( 1, 8 ),  PFC_FNS( 1, 9 ),  PFC_FNS( 1, 10 )),
    SETPFC( PQCR,   PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),
                    PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" )),
    SETPFC( PRCR,   PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),
                    PFC_FNS( 0, 9 ),  PFC_FN( "BSC" ),  PFC_FNS( 0, 12 ), PFC_FNS( 3, 9 )),
    SETPFC( PSCR,   PFC_FN( "***" ),  PFC_FN( "VIO" ),  PFC_FNS( 4, 4 ),  PFC_FNS( 0, 4 ),
                    PFC_FNS( 0, 5 ),  PFC_FNS( 0, 5 ),  PFC_FNS( 1, 7 ),  PFC_FNS( 1, 7 )),
    SETPFC( PTCR,   PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),
                    PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" )),
    SETPFC( PUCR,   PFC_FN( "DMAC" ), PFC_FN( "DMAC" ), PFC_FN( "FSI" ),  PFC_FN( "FSI" ),
                    PFC_FN( "FSI" ),  PFC_FN( "FSI" ),  PFC_FN( "FSI" ),  PFC_FN( "FSI" )),
    SETPFC( PVCR,   PFC_FNS( 3, 5 ),  PFC_FNS( 3, 5 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),
                    PFC_FNS( 2, 0 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 4, 7 ),  PFC_FNS( 4, 3 )),
    SETPFC( PWCR,   PFC_FNS( 4, 1 ),  PFC_FNS( 4, 1 ),  PFC_FNS( 4, 1 ),  PFC_FNS( 4, 1 ),
                    PFC_FNS( 0, 1 ),  PFC_FNS( 0, 1 ),  PFC_FNS( 0, 1 ),  PFC_FNS( 0, 1 )),
    SETPFC( PXCR,   PFC_FNS( 0, 2 ),  PFC_FNS( 0, 2 ),  PFC_FNS( 2, 9 ),  PFC_FNS( 2, 8 ),
                    PFC_FNS( 2, 7 ),  PFC_FN( "TSIF" ), PFC_FN( "MMC" ),  PFC_FN( "MMC" )),
    SETPFC( PYCR,   PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" ),
                    PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" )),
    SETPFC( PZCR,   PFC_FNS( 1, 5 ),  PFC_FNS( 1, 4 ),  PFC_FNS( 1, 3 ),  PFC_FNS( 1, 2 ),
                    PFC_FNS( 1, 1 ),  PFC_FN( "INTC" ), PFC_FN( "INTC" ), PFC_FN( "INTC" )),
};

static STREGCTRL psel[] = {
    SETREG( PSELA, TWORD,
            EREG( "15-14", 2, "BSC", "KEYSC", "ATAPI", "Error" ),
            EREG( "13",    1, "MMC", "SDHI" ),
            EREG( "12",    1, "DMAC", "IrDA" ),
            DREG( "11",    1 ),
            EREG( "10",    1, "VIO", "SCIF5" ),
            EREG( "9",     1, "VIO", "SCIF5" ),
            EREG( "8",     1, "BSC(A25)", "BSC(BS)" ),
            EREG( "7",     1, "LCDC", "SCIF0" ),
            EREG( "6",     1, "LCDC", "SCIF0" ),
            EREG( "5",     1, "BSC", "LCDC" ),
            DREG( "4",     1 ),
            EREG( "3-2",   2, "LCDC", "SCIF4" ),
            EREG( "1-0",   2, "BSC", "TPU(TPUTO3)", "TPU(TPUTI3)" )),
    SETREG( PSELB, TWORD,
            DREG( "15",    1 ),
            EREG( "14",    1, "IRQ3", "SCIF3" ),
            EREG( "13",    1, "IRQ4", "SCIF3" ),
            EREG( "12",    1, "IRQ5", "SCIF3" ),
            EREG( "11",    1, "IRQ6", "SCIF3" ),
            EREG( "10",    1, "IRQ7", "SCIF3" ),
            EREG( "9-8",   2, "VOU", "SCIF3", "EtherC" ),
            EREG( "7-6",   2, "VIO1", "VIO0", "ATAPI" ),
            EREG( "5-4",   2, "VOU", "SCIF2" ),
            EREG( "3-2",   2, "VOU", "SCIF3", "EtherC" ),
            EREG( "1-0",   2, "VOU", "SCIF3", "EtherC" )),
    SETREG( PSELC, TWORD,
            EREG( "15-14",  2, "FSI", "MSIOF" ),
            EREG( "13-12",  2, "VOU", "MSIOF" ),
            EREG( "11-10",  2, "VOU", "MSIOF(SS)", "MSIOF(SCK)" ),
            EREG( "9-8",    2, "VOU", "SCIF1", "EtherC" ),
            EREG( "7-6",    2, "LCDC", "SCIF2" ),
            EREG( "5-4",    2, "LCDC", "SCIF2" ),
            DREG( "3",      1 ),
            EREG( "2",      1, "TSIF", "EtherC" ),
            EREG( "1",      1, "TSIF", "EtherC" ),
            EREG( "0",      1, "TSIF", "EtherC" )),
    SETREG( PSELD, TWORD,
            EREG( "15-14",  2, "VOU", "SCIF2" ),
            EREG( "13-12",  2, "VOU", "SCIF2" ),
            EREG( "11",     1, "VOU" ),
            EREG( "10-9",   2, "LCDC(0)", "LCDC(1)" ),
            EREG( "8",      1, "LCDC" ),
            EREG( "7-6",    2, "FSI", "MSIOF(1)", "MSIOF(2)" ),
            EREG( "5",      1, "LCDC" ),
            EREG( "4",      1, "VOU" ),
            EREG( "3-2",    2, "LCDC", "SCIF4" ),
            EREG( "1-0",    2, "BSC", "TPU", "ATAPI" )),
    SETREG( PSELE, TWORD,
            EREG( "15-14",  2, "BSC", "TPU", "ATAPI" ),
            EREG( "13-12",  2, "MMC", "SDHI", "ATAPI" ),
            EREG( "11",     1, "BSC", "ATAPI" ),
            EREG( "10",     1, "FSI" ),
            EREG( "9-8",    2, "VIO", "TPU", "ATAPI" ),
            EREG( "7-6",    2, "VOU", "SCIF3", "EtherC" ),
            EREG( "5-4",    2, "VOU", "MSIOF" ),
            EREG( "3-2",    2, "FSI", "MSIOF" ),
            EREG( "1",      1, "FSI" ),
            EREG( "0",      1, "FSI" )),
};

static STSHPFCCTRL pfc_ctrl = {
    .pPxCR      = pxcr,
    .nPxCRSize  = ARRAY_SIZE( pxcr ),
    .pPSEL      = psel,
    .nPSELSize  = ARRAY_SIZE( psel ),
};

static void cmdinit( const STCMD *pCmd )
{
    //----------------------
    // PFC
    //----------------------
    if ( pCmd == &cmd_pfc ) {

        SetCmdData( "pfc" , (void*)&pfc_ctrl );
        SetRegs( PACR,    0xA4050100 );
        SetRegs( PBCR,    0xA4050102 );
        SetRegs( PCCR,    0xA4050104 );
        SetRegs( PDCR,    0xA4050106 );
        SetRegs( PECR,    0xA4050108 );
        SetRegs( PFCR,    0xA405010A );
        SetRegs( PGCR,    0xA405010C );
        SetRegs( PHCR,    0xA405010E );
        SetRegs( PJCR,    0xA4050110 );
        SetRegs( PKCR,    0xA4050112 );
        SetRegs( PLCR,    0xA4050114 );
        SetRegs( PMCR,    0xA4050116 );
        SetRegs( PNCR,    0xA4050118 );
        SetRegs( PQCR,    0xA405011A );
        SetRegs( PRCR,    0xA405011C );
        SetRegs( PSCR,    0xA405011E );
        SetRegs( PTCR,    0xA4050140 );
        SetRegs( PUCR,    0xA4050142 );
        SetRegs( PVCR,    0xA4050144 );
        SetRegs( PWCR,    0xA4050146 );
        SetRegs( PXCR,    0xA4050148 );
        SetRegs( PYCR,    0xA405014A );
        SetRegs( PZCR,    0xA405014C );
        SetRegs( PSELA,   0xA405014E );
        SetRegs( PSELB,   0xA4050150 );
        SetRegs( PSELC,   0xA4050152 );
        SetRegs( PSELD,   0xA4050154 );
        SetRegs( PSELE,   0xA4050156 );
        SetRegs( HIZCRA,  0xA4050158 );
        SetRegs( HIZCRB,  0xA405015A );
        SetRegs( HIZCRC,  0xA405015C );
        SetRegs( HIZCRD,  0xA405015E );
        SetRegs( MSELCRA, 0xA4050180 );
        SetRegs( MSELCRB, 0xA4050182 );
        SetRegs( PULCR,   0xA4050184 );
        SetRegs( DRVCRA,  0xA405018A );
        SetRegs( DRVCRB,  0xA405018C );
        SetRegs( DRVCRC,  0xA405018E );
    }
    else if ( pCmd == &cmd_io ) {
        SetRegs( PADR,    0xA4050120 );
        SetRegs( PBDR,    0xA4050122 );
        SetRegs( PCDR,    0xA4050124 );
        SetRegs( PDDR,    0xA4050126 );
        SetRegs( PEDR,    0xA4050128 );
        SetRegs( PFDR,    0xA405012A );
        SetRegs( PGDR,    0xA405012C );
        SetRegs( PHDR,    0xA405012E );
        SetRegs( PJDR,    0xA4050130 );
        SetRegs( PKDR,    0xA4050132 );
        SetRegs( PLDR,    0xA4050134 );
        SetRegs( PMDR,    0xA4050136 );
        SetRegs( PNDR,    0xA4050138 );
        SetRegs( PQDR,    0xA405013A );
        SetRegs( PRDR,    0xA405013C );
        SetRegs( PSDR,    0xA405013E );
        SetRegs( PTDR,    0xA4050160 );
        SetRegs( PUDR,    0xA4050162 );
        SetRegs( PVDR,    0xA4050164 );
        SetRegs( PWDR,    0xA4050166 );
        SetRegs( PXDR,    0xA4050168 );
        SetRegs( PYDR,    0xA405016A );
        SetRegs( PZDR,    0xA405016C );
    }
    else if ( pCmd == &cmd_mstp ) {
        SetRegs( STBCR,   0xA4150020 );
        SetRegs( MSTPCR0, 0xA4150030 );
        SetRegs( MSTPCR1, 0xA4150034 );
        SetRegs( MSTPCR2, 0xA4150038 );
    }
    else if ( pCmd == &cmd_bsc ) {
        SetRegs( CMNCR,   0xFEC10000 );
        SetRegs( CS0BCR,  0xFEC10004 );
        SetRegs( CS4BCR,  0xFEC10010 );
        SetRegs( CS5ABCR, 0xFEC10014 );
        SetRegs( CS5BBCR, 0xFEC10018 );
        SetRegs( CS6ABCR, 0xFEC1001C );
        SetRegs( CS6BBCR, 0xFEC10020 );
        SetRegs( CS0WCR,  0xFEC10024 );
        SetRegs( CS4WCR,  0xFEC10030 );
        SetRegs( CS5AWCR, 0xFEC10034 );
        SetRegs( CS5BWCR, 0xFEC10038 );
        SetRegs( CS6AWCR, 0xFEC1003C );
        SetRegs( CS6BWCR, 0xFEC10040 );
    }
}

static void init( const STCPU *pCpu )
{
    EnableCmd( "pfc" );
    EnableCmd( "io" );
    EnableCmd( "mstp" );
    EnableCmd( "bsc" );
}

INSTALL_CPU( SH7724 )
