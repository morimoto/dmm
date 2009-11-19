/************************************************************************


                      io ( if cpu is sh )


************************************************************************/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"

static STHELPMSG s_Help[] = {
    {
        "io"  , "dump, edit IO port register" ,
        "\n"
        "dmm io [rwsmh] [param]\n\n"
        "[option]          [param]\n"
        " -r : read         a-z  :  PACR - PZCR\n"
        " -w : write\n"
        " -s : set bit\n"
        " -c : clear bit\n"
        " -h : print help\n"
        "\n"
        "ex) case a is 0x12\n"
        "dmm io a            => 0x12\n"
        "dmm io -r a         => 0x12\n"
        "dmm io -w a 0x56    => 0x56\n"
        "dmm io -s a 0x80    => 0x92\n"
        "dmm io -c a 0x0f    => 0x10\n"
        "\n"
    },
    HELPMSGEND
};

//=====================================
//
//               cmd
//
//=====================================
static bool cmd( int nArgc, char *pstrArgv[] )
{
    return CmdCommon( nArgc , pstrArgv , TBYTE , IO_START , IO_END );
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
INSTALL_CMD( io )
