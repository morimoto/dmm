/*=============================================

                     sh7786

2009/03  morimoto
==============================================*/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"
#include "cmd/cmdlist.h"

static void cmdinit( const STCMD *pCmd )
{
    if ( pCmd == &cmd_pfc ) {
        SetRegs( PACR,  0xFFCC0000 );
        SetRegs( PBCR, 0xFFCC0002 );
        SetRegs( PCCR, 0xFFCC0004 );
        SetRegs( PDCR, 0xFFCC0006 );
        SetRegs( PECR, 0xFFCC0008 );
        SetRegs( PFCR, 0xFFCC000A );
        SetRegs( PGCR, 0xFFCC000C );
        SetRegs( PHCR, 0xFFCC000E );
        SetRegs( PJCR, 0xFFCC0010 );
        SetRegs( P1MSELR, 0xFFCC0080 );
        SetRegs( P2MSELR, 0xFFCC0082 );
    }
    else if ( pCmd == &cmd_io ) {
        SetRegs( PADR, 0xFFCC0020 );
        SetRegs( PBDR, 0xFFCC0022 );
        SetRegs( PCDR, 0xFFCC0024 );
        SetRegs( PDDR, 0xFFCC0026 );
        SetRegs( PEDR, 0xFFCC0028 );
        SetRegs( PFDR, 0xFFCC002A );
        SetRegs( PGDR, 0xFFCC002C );
        SetRegs( PHDR, 0xFFCC002E );
        SetRegs( PJDR, 0xFFCC0030 );
    }
    else if ( pCmd == &cmd_mstp ) {
        SetRegs( MSTPCR0, 0xFFC40030 );
        SetRegs( MSTPCR1, 0xFFC40034 );
    }
    else if ( pCmd == &cmd_bsc ) {
        SetRegs( CS0BCR, 0xFF802000 );
        SetRegs( CS1BCR, 0xFF802010 );
        SetRegs( CS2BCR, 0xFF802020 );
        SetRegs( CS3BCR, 0xFF802030 );
        SetRegs( CS4BCR, 0xFF802040 );
        SetRegs( CS5BCR, 0xFF802050 );
        SetRegs( CS6BCR, 0xFF802060 );
        SetRegs( CS0WCR, 0xFF802008 );
        SetRegs( CS1WCR, 0xFF802018 );
        SetRegs( CS2WCR, 0xFF802028 );
        SetRegs( CS3WCR, 0xFF802038 );
        SetRegs( CS4WCR, 0xFF802048 );
        SetRegs( CS5WCR, 0xFF802058 );
        SetRegs( CS6WCR, 0xFF802068 );
    }
}

static void init( const STCPU *pCpu )
{
    EnableCmd( "pfc" );
    EnableCmd( "io" );
    EnableCmd( "mstp" );
    EnableCmd( "bsc" );
}

INSTALL_CPU( SH7786 )
