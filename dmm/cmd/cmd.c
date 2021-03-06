/************************************************************************


			cmd

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include "cmdlist.c"

//======================================================================
//
//
//			global function
//
//
//======================================================================


//=====================================
//
//	Usage
//
//=====================================
bool DetailUsage(const char *pCmd)
{
	STHELPMSG *phelp;
	int i;

	for (i = 0; i < GetCmdListSize(); i++) {

		phelp = GetCmdList(i)->pHelp;
		while (phelp->pstrCmd) {
			if (CMD_HIT(phelp->pstrCmd, pCmd)) {
				printf("%s\n", phelp->pstrDetail);
				return true;
			}
			phelp++;
		}
	}

	Usage("help");
	return true;
}

bool Usage(const char *pMsg)
{
	int i = 0;
	STHELPMSG *phelp = NULL;

	printf("[ %s ]\n", pMsg);

	// print --XXX option
	for (i = 0; i < GetCmdListSize(); i++) {

		phelp = GetCmdList(i)->pHelp;
		while (phelp->pstrCmd) {
			if ((strlen(phelp->pstrCmd) > 2) &&
			    ('-' == phelp->pstrCmd[0])   &&
			    ('-' == phelp->pstrCmd[1]))
				printf("%-15s : %s\n",
				       phelp->pstrCmd, phelp->pstrUsage);

			phelp++;
		}
	}

	// print other help
	for (i = 0; i < GetCmdListSize(); i++) {

		phelp = GetCmdList(i)->pHelp;
		while (phelp->pstrCmd) {
			if ('-' != phelp->pstrCmd[0])
				printf("%-15s : %s\n",
				       phelp->pstrCmd, phelp->pstrUsage);

			phelp++;
		}
	}

	return true;
}

//=====================================
//
//	IsCmdHit
//
//=====================================
bool _IsCmdHit(const char *pstrCmd, STHELPMSG *pHelp)
{
	while (pHelp->pstrCmd) {
		if (CMD_HIT(pHelp->pstrCmd, pstrCmd))
			return true;
		pHelp++;
	}
	return false;
}

//=====================================
//
//	CmdGetDataSize
//
// copy from uboot
//=====================================
enum ESTYPE CmdGetDataSize(char *pstrArg, enum ESTYPE EDefaultType)
{
	// Check for a size specification .b, .w or .l.
	size_t len = strlen(pstrArg);

	if (len > 2 && pstrArg[len - 2] == '.') {
		switch (pstrArg[len - 1]) {
		case 'b': return TBYTE;
		case 'w': return TWORD;
		case 'l': return TLONG;
		default:  return TERR;
		}
	}
	return EDefaultType;
}

//=====================================
//
//	GetCmdListSize
//
//=====================================
int GetCmdListSize(void)
{
	return s_nCmdListSize;
}

//=====================================
//
//	GetCmdList
//
//=====================================
const STCMD *GetCmdList(int ixCmd)
{
	if (ixCmd < 0 ||
	    GetCmdListSize() <= ixCmd) {
		Error("Cmd List index error");
		return NULL;
	}

	return s_tblCmdList[ixCmd];
}
