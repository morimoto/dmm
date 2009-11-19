/************************************************************************


                      mstp ( if cpu is sh )


************************************************************************/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"

static STHELPMSG s_Help[] = {
    {
        "mstp"  , "dump, edit MSTP register" ,
        "\n"
        "dmm mstp [rwsmh][param]\n\n"
        "[option]           [param]\n"
        " -r : read          stb  : STBCR\n"
        " -w : write         0-2  : MSTPCR0 - MSTPCR2\n"
        " -s : set bit\n"
        " -c : clear bit\n"
        " -h : print help\n"
        "\n"
        "ex) case stb is 0x12345678\n"
        "dmm mstp stb                => 0x12345678\n"
        "dmm mstp -r stb             => 0x12345678\n"
        "dmm mstp -w stb 0x56781234  => 0x56781234\n"
        "dmm mstp -s stb 0x80000001  => 0x92345679\n"
        "dmm mstp -c stb 0x000f000f  => 0x12305670\n"
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
    return CmdCommon( nArgc , pstrArgv , TLONG , MSTP_START , MSTP_END );
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
INSTALL_CMD( mstp )
