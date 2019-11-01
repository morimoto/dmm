/************************************************************************


			md

2008/09  morimoto
************************************************************************/
#include "dmm.h"
#include <unistd.h>

//=====================================
//
//	help
//
//=====================================
static STHELPMSG s_Help[] = {
	{
		"md", "memory dump",
		"dmm md[.d, .w, .l] [-cs] addr [len]\n"
		"    -c    : dump in 1 column\n"
		"    -s    : silence output\n"
		"    addr  : address\n"
		"    len   : length\n"
		" ex) dmm md.b 0x88000000\n"
		"     dmm md.w 0x88000000 0x20\n"
		"     dmm md   0x88000ff0 32    # it mean LONG"
	},
	HELPMSGEND
};

//=====================================
//
//	mem_dump
//
//=====================================
static int mem_dump_single_column(STMEMCTRL *pCtrl,
				  int silence,
				  uintptr_t ulAddr,
				  u32 ulLen)
{
	u32 inc;
	uintptr_t l;

	DMSG("%s\n", __func__);

	switch (pCtrl->EStype) {
	case TBYTE: inc = 1; break;
	case TWORD: inc = 2; break;
	case TLONG: inc = 4; break;
	case TERR:
	default:
		printf("unknown stype\n");
		return false;
	}

	for (l = ulAddr; l < ulAddr + ulLen; l += inc) {

		if (!silence) {
			BLUE;
			printf("%p   ", (void *)l);
			CLAR;
		}

		if (!pCtrl->fnDump(l))
			break;
		printf("\n");
	}

	return true;
}

static int mem_dump_mult_column(STMEMCTRL *pCtrl,
				 int silence,
				 uintptr_t ulAddr,
				 u32 ulLen)
{
	u32 i = 0;
	uintptr_t l = 0;
	const char *space = NULL;

	DMSG("%s\n", __func__);

	switch (pCtrl->EStype) {
	case TBYTE: space = " ";	break;
	case TWORD: space = "   ";	break;
	case TLONG: space = "       ";	break;
	case TERR:
	default:
		printf("dump error\n");
		return false;
	}

	//----------------------
	// draw offset line
	//----------------------
	if (!silence) {
		BLUE;
		printf("             ");

		for (i = 0; i <= 0xF; i += pCtrl->nIncSize)
			printf("%x%s ", i, space);

		CLAR;
		printf("\n");
	}

	//----------------------
	// draw memory
	//----------------------
	for (l = (ulAddr & ~(uintptr_t)0xF); l < ulAddr + ulLen; l += 0x10) {

		if (!silence) {
			BLUE;
			printf("%p   ", (void *)l);
			CLAR;
		}

		for (i = 0; i <= 0xF; i += pCtrl->nIncSize) {

			if ((l + i) >= ulAddr+ulLen)
				break;

			if ((l + i) <  ulAddr) {
				printf(" %s ", space);
			} else {
				if (!pCtrl->fnDump(l + i))
					break;
				printf(" ");
			}

		}
		printf("\n");
	}

	return true;
}

//=====================================
//
//	cmd
//
//=====================================
static bool cmd(int nArgc, char *pstrArgv[])
{
	uintptr_t	addr;
	enum ESTYPE	stype  = CmdGetDataSize(pstrArgv[0], TLONG);
	STMEMCTRL	*pmctrl = NULL;
	bool		ret    = false;
	u32		len;
	int		res;
	int		silence = false;
	int		(*func)(STMEMCTRL *pCtrl, int silence,
				uintptr_t ulAddr, u32 ulLen);

	func = mem_dump_mult_column;

	while ((res = getopt(nArgc, pstrArgv, "cs")) != -1) {
		switch (res) {
		case 'c':
			func = mem_dump_single_column;
			break;
		case 's':
			silence = true;
			break;
		default:
			return Usage("unknown option");
		}
	}

	nArgc    -= optind;
	pstrArgv += optind;

	if (stype == TERR)
		return Usage("type error");

	if (!MemInit())
		return Error("mem ctrl init error");

	pmctrl = GetMemCtrl(stype);
	if (!pmctrl) {
		Error("get mem ctrl error");
		goto error;
	}

	addr = GetAddress(pstrArgv[0]);
	if (!addr) {
		Error("addr error");
		goto error;
	}

	if (addr & (pmctrl->nIncSize-1)) {
		Error("stype and addr error");
		goto error;
	}

	if (nArgc == 1) {
		func = mem_dump_single_column;
		len = 1;
	} else {
		len = GetData(pstrArgv[1]);
	}

	ret = func(pmctrl, silence, addr, len);

error:
	MemExit();

	return ret;
}

//=====================================
//
//	ishit
//
//=====================================
static bool
ishit(int nArgc, char *pstrArgv[])
{
	if (CMD_HIT(pstrArgv[0], "md.b") ||
	    CMD_HIT(pstrArgv[0], "md.w") ||
	    CMD_HIT(pstrArgv[0], "md.l") ||
	    IsCmdHit())
		return true;

	return false;
}
INSTALL_CMD(md)
