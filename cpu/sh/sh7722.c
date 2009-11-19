/*=============================================

                     sh7722

2008/09  morimoto
==============================================*/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"
#include "cmd/cmdlist.h"
#include "cmd/cmd-sh-pfc-v.h"

static STPFCCTRL pxcr[] = {

    //              7                 6                 5                4
    //              3                 2                 1                0
    SETPFC( PACR,   PFC_FNS( 3, 3 ),  PFC_FNS( 3, 3 ),  PFC_FNS( 3, 3 ),  PFC_FN( "VIO" ),
                    PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FN( "VIO" ),  PFC_FNS( 3, 4 )),
    SETPFC( PBCR,   PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ),
                    PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" )),
    SETPFC( PCCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "SBSC" ), PFC_FN( "SBSC" ),
                    PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "***" ),  PFC_FN( "SBSC" )),
    SETPFC( PDCR,   PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FNS( 0, 7 ),
                    PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" ), PFC_FN( "SDHI" )),
    SETPFC( PECR,   PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),  PFC_FN( "BSC" ),
                    PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "IRQ5" ), PFC_FNS( 0, 5 )),
    SETPFC( PFCR,   PFC_FN( "***" ),  PFC_FNS( 1, 6 ),  PFC_FNS( 1, 5 ),  PFC_FNS( 1, 4 ),
                    PFC_FNS( 1, 3 ),  PFC_FNS( 1, 2 ),  PFC_FNS( 1, 1 ),  PFC_FNS( 1, 0 )),
    SETPFC( PGCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "AUD" ),
                    PFC_FN( "AUD" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" ),  PFC_FN( "AUD" )),
    SETPFC( PHCR,   PFC_FNS( 3, 11 ), PFC_FNS( 4, 3 ),  PFC_FN( "LCDC" ), PFC_FN( "LCDC" ),
                    PFC_FN( "LCDC" ), PFC_FNS( 3, 12 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 )),
    SETPFC( PJCR,   PFC_FN( "CPG" ),  PFC_FN( "***" ),  PFC_FN( "CPG" ),  PFC_FN( "***" ),
                    PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "IRQ1" ), PFC_FN( "IRQ0" )),
    SETPFC( PKCR,   PFC_FN( "***" ),  PFC_FNS( 2, 4 ),  PFC_FNS( 2, 3 ),  PFC_FNS( 2, 2 ),
                    PFC_FNS( 2, 1 ),  PFC_FNS( 2, 0 ),  PFC_FNS( 1, 15 ), PFC_FNS( 1, 14 )),
    SETPFC( PLCR,   PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ),
                    PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 )),
    SETPFC( PMCR,   PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ),
                    PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 )),
    SETPFC( PNCR,   PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ),
                    PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" ), PFC_FN( "SBSC" )),
    SETPFC( PQCR,   PFC_FN( "***" ),  PFC_FNS( 1, 13 ), PFC_FNS( 1, 12 ), PFC_FNS( 1, 11 ),
                    PFC_FNS( 1, 10 ), PFC_FNS( 1, 9 ),  PFC_FNS( 1, 8 ),  PFC_FNS( 1, 7 )),
    SETPFC( PRCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "**" ),   PFC_FN( "LCDC" ),
                    PFC_FNS( 3, 9 ),  PFC_FN( "BSC" ),  PFC_FN( "LCDC" ), PFC_FNS( 3, 11 )),
    SETPFC( PSCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 3, 8 ),
                    PFC_FNS( 3, 7 ),  PFC_FNS( 3, 6 ),  PFC_FN( "SCIF" ), PFC_FN( "SCIF" )),
    SETPFC( PTCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 4, 6 ),
                    PFC_FN( "FLCTL" ),PFC_FN( "FLCTL" ),PFC_FN( "DMAC" ), PFC_FN( "FLCTL" )),
    SETPFC( PUCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 4, 7 ),
                    PFC_FNS( 4, 8 ),  PFC_FNS( 4, 9 ),  PFC_FNS( 4, 6 ),  PFC_FNS( 4, 6 )),
    SETPFC( PVCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 2, 6 ),
                    PFC_FNS( 2, 6 ),  PFC_FNS( 2, 6 ),  PFC_FNS( 2, 6 ),  PFC_FNS( 2, 6 )),
    SETPFC( PWCR,   PFC_FN( "***" ),  PFC_FNS( 3, 1 ),  PFC_FNS( 3, 1 ),  PFC_FNS( 3, 1 ),
                    PFC_FNS( 3, 1 ),  PFC_FNS( 3, 1 ),  PFC_FNS( 3, 2 ),  PFC_FNS( 3, 2 )),
    SETPFC( PXCR,   PFC_FN( "***" ),  PFC_FN( "BSC" ),  PFC_FN( "LCDC" ), PFC_FN( "LCDC" ),
                    PFC_FN( "LCDC" ), PFC_FN( "LCDC" ), PFC_FNS( 3, 14 ), PFC_FNS( 3, 14 )),
    SETPFC( PYCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FN( "KEY" ),  PFC_FN( "KEY" ),
                    PFC_FN( "kEY" ),  PFC_FN( "KEY" ),  PFC_FN( "KEY" ),  PFC_FN( "KEY" )),
    SETPFC( PZCR,   PFC_FN( "***" ),  PFC_FN( "***" ),  PFC_FNS( 0, 1 ),  PFC_FN( "KEY" ),
                    PFC_FN( "KEY" ),  PFC_FN( "KEY" ),  PFC_FNS( 0, 0 ),  PFC_FN( "***" )),
};

static STREGCTRL psel[] = {
    SETREG( PSELA, TWORD,
            EREG( "15",    1, "KEYSC", "IRQ6" ),
            EREG( "14",    1, "KEYSC", "IRQ7" ),
            DREG( "13-10", 4 ),
            EREG( "9",     1, "IRQ4", "BSC" ),
            DREG( "8-5",   4 ),
            EREG( "4",     1, "IRQ2", "SDHI" ),
            DREG( "3-0",   4 )),
    SETREG( PSELB, TWORD,
            EREG( "15",    1, "SIO", "SIU" ),
            EREG( "14",    1, "SIO", "SIU" ),
            EREG( "13",    1, "SIO", "SIU" ),
            EREG( "12",    1, "SIO", "SIU" ),
            EREG( "11",    1, "SIO", "SIU" ),
            EREG( "10",    1, "SIO", "SIU" ),
            EREG( "9",     1, "SIO", "SIU" ),
            EREG( "8",     1, "SIOF0", "IRQ3" ),
            EREG( "7",     1, "SIOF0", "IrDA" ),
            EREG( "6",     1, "SIOF0", "IrDA" ),
            EREG( "5",     1, "SIOF0", "TSIF" ),
            EREG( "4",     1, "SIOF0", "TSIF" ),
            EREG( "3",     1, "SIOF0", "TSIF" ),
            EREG( "2",     1, "SIOF0", "SIM" ),
            EREG( "1",     1, "SIU", "SIOF1" ),
            EREG( "0",     1, "SIU", "SIOF1" )),
    SETREG( PSELC, TWORD,
            EREG( "15",     1, "SIU", "SIOF1" ),
            EREG( "14",     1, "SIU", "SIOF1" ),
            EREG( "13",     1, "SIU", "SIOF1" ),
            EREG( "12",     1, "SIU", "SIOF1" ),
            EREG( "11",     1, "SIU", "SIOF1" ),
            DREG( "10-1",   10 ),
            EREG( "0",      1, "FLCTL", "VIO" )),
    SETREG( PSELD, TWORD,
            DREG( "15-14",  2 ),
            EREG( "13",     1, "VIO", "SCIF2" ),
            EREG( "12",     1, "VIO", "SCIF1" ),
            EREG( "11",     1, "VIO", "SCIF1" ),
            EREG( "10",     1, "VIO", "LCDC" ),
            EREG( "9",      1, "SIO/SIU(0)", "SIU(1)" ),
            EREG( "8",      1, "SCIF0", "TPU" ),
            EREG( "7",      1, "SCIF0", "SIU" ),
            EREG( "6",      1, "SCIF0", "SIU" ),
            EREG( "5",      1, "BSC", "LCDC" ),
            DREG( "4",      1 ),
            EREG( "3",      1, "LCDC(0)", "LCDC(1)" ),
            EREG( "2",      1, "LCDC(0)", "LCDC(1)" ),
            DREG( "1",      1 ),
            EREG( "0",      1, "LCDC", "VOU" )),
    SETREG( PSELE, TWORD,
            EREG( "15",     1, "SIOF0/IRQ3", "SIM" ),
            EREG( "14",     1, "SIOF0", "SIM" ),
            EREG( "13",     1, "SIOF0", "TSIF" ),
            EREG( "12",     1, "LCDC", "DMAC" ),
            EREG( "11",     1, "SIU/SIOF1", "SIU" ),
            DREG( "10-4",   7 ),
            EREG( "3",      1, "FLCTL", "VIO" ),
            EREG( "2",      1, "FLCTL", "VIO" ),
            EREG( "1",      1, "FLCTL", "VIO" ),
            EREG( "0",      1, "FLCTL", "VIO")),
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
       SetRegs( PSELE   , 0xA4050156 );
       SetRegs( HIZCRA  , 0xA4050158 );
       SetRegs( HIZCRB  , 0xA405015A );
       SetRegs( HIZCRC  , 0xA405015C );
       SetRegs( MSELCRA , 0xA4050180 );
       SetRegs( MSELCRB , 0xA4050182 );
       SetRegs( PULCR   , 0xA4050184 );
       SetRegs( SBSCR   , 0xA4050186 );
       SetRegs( DRVCR   , 0xA405018A );
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

INSTALL_CPU( SH7722 )
