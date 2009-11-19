/************************************************************************


                      bsc ( if cpu is sh )


************************************************************************/
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"

static STHELPMSG s_Help[] = {
    {
        "bsc"  , "dump, edit BSC register" ,
        "\n"
        "dmm bsc [rwsmh] [param]\n\n"
        "[option]           [param]\n"
        " -r : read          cr    :  CMNCR\n"
        " -w : write         0b-6b :  CS0BCR - CS6BCR\n"
        " -s : set bit       0w-6w :  CS0WCR - CS6WCR\n"
        " -c : clear bit     rbw   :  RBWTCNT\n"
        " -h : print help\n"
        "\n"
        "[area]\n"
        "0b 1b 2b 3b 4b 5b 5ab 5bb 6b 6ab 6bb\n"
        "0w 1w 2w 3w 4w 5w 5aw 5bw 6w 6aw 6bw\n"
        "\n"
        "ex) case cr is 0x12345678\n"
        "dmm bsc cr                => 0x12345678\n"
        "dmm bsc -r cr             => 0x12345678\n"
        "dmm bsc -w cr 0x56781234  => 0x56781234\n"
        "dmm bsc -s cr 0x80000001  => 0x92345679\n"
        "dmm bsc -c cr 0x000f000f  => 0x12305670\n"
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
    return CmdCommon( nArgc , pstrArgv , TLONG , BSC_START , BSC_END );
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
INSTALL_CMD( bsc )
