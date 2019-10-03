/************************************************************************


			version

2008/09  morimoto
************************************************************************/
#include "dmm.h"

//======================================================================
//
//
//			help
//
//
//======================================================================
static STHELPMSG s_Help[] = {
	{"--version",	"print version (-v)",	"dmm --version"},
	{"-v",		"print version",	"dmm -v"},
	HELPMSGEND
};

//=====================================
//
//	cmd
//
//=====================================
static bool cmd(int nArgc, char *pstrArgv[])
{
	printf("version %s\n", VERSION);
	return true;
}

//=====================================
//
//	ishit
//
//=====================================
static bool
ishit(int nArgc, char *pstrArgv[])
{
	if (IsCmdHit() &&
	     (nArgc == 1))
		return true;

	return false;
}
INSTALL_CMD(version)
