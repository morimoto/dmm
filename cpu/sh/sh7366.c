/*=============================================

                     sh7366

2008/09  morimoto
==============================================*/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"
#include "cmd/cmdlist.h"

static void cmdinit( const STCMD *pCmd )
{
    if ( pCmd == &cmd_pfc ) {
        SetRegs( PACR    , 0xA4050100 );
        SetRegs( PBCR    , 0xA4050102 );
        SetRegs( PCCR    , 0xA4050104 );
        SetRegs( PDCR    , 0xA4050106 );
        SetRegs( PECR    , 0xA4050108 );
        SetRegs( PGCR    , 0xA405010C );
        SetRegs( PHCR    , 0xA405010E );
        SetRegs( PJCR    , 0xA4050110 );
        SetRegs( PKCR    , 0xA4050112 );
        SetRegs( PLCR    , 0xA4050114 );
        SetRegs( PMCR    , 0xA4050116 );
        SetRegs( PNCR    , 0xA4050118 );
        SetRegs( PQCR    , 0xA405011A );
        SetRegs( PRCR    , 0xA405011C );
        SetRegs( PTCR    , 0xA4050140 );
        SetRegs( PUCR    , 0xA4050142 );
        SetRegs( PSELA   , 0xA405014E );
        SetRegs( PSELB   , 0xA4050150 );
        SetRegs( PSELC   , 0xA4050152 );
        SetRegs( HIZCRA  , 0xA4050158 );
        SetRegs( HIZCRB  , 0xA405015A );
        SetRegs( HIZCRC  , 0xA405015C );
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
        SetRegs( PGDR    , 0xA405012C );
        SetRegs( PHDR    , 0xA405012E );
        SetRegs( PJDR    , 0xA4050130 );
        SetRegs( PKDR    , 0xA4050132 );
        SetRegs( PLDR    , 0xA4050134 );
        SetRegs( PMDR    , 0xA4050136 );
        SetRegs( PNDR    , 0xA4050138 );
        SetRegs( PQDR    , 0xA405013A );
        SetRegs( PRDR    , 0xA405013C );
        SetRegs( PTDR    , 0xA4050160 );
        SetRegs( PUDR    , 0xA4050162 );
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
        SetRegs( CS0WCR  , 0xFEC10024 );
        SetRegs( CS4WCR  , 0xFEC10030 );
        SetRegs( CS5AWCR , 0xFEC10034 );
        SetRegs( CS5BWCR , 0xFEC10038 );
        SetRegs( CS6AWCR , 0xFEC1003C );
    }
}

static void init( const STCPU *pCpu )
{
    EnableCmd( "pfc" );
    EnableCmd( "io" );
    EnableCmd( "mstp" );
    EnableCmd( "bsc" );
}

INSTALL_CPU( SH7366 )
