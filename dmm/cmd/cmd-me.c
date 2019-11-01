/************************************************************************


			me

2008/09  morimoto
************************************************************************/
#include "dmm.h"

//=====================================
//
//	help
//
//=====================================
static STHELPMSG s_Help[] = {
	{
		"me", "memory edit",
		"dmm me[.d, .w, .l] addr data\n"
		"    addr  : address\n"
		"    data  : data\n"
		"ex) dmm me.w 0x89000000 0x1234\n"
		"    dmm me   0x89000000 0x12345678  # it mean LONG\n"
		"\n"
		"you can use mask and set by -m option\n"
		"dmm me -m 0x89000000 0xFFFF0000 0x12340000\n"
	},
	HELPMSGEND
};

//=====================================
//
//	cmd
//
//=====================================
static bool cmd(int nArgc, char *pstrArgv[])
{

	uintptr_t	addr;
	enum ESTYPE	stype;
	u32		data;
	u32		data_check = 0xFFFFFFFF;
	u32		data_mask = 0;
	STMEMCTRL	*pmctrl = NULL;
	bool		ret = false;
	int		i = 0;

	stype = CmdGetDataSize(pstrArgv[i++], TLONG);

	if (strcmp("-m", pstrArgv[i]) == 0)
		data_mask = (u32)i++;

	addr = GetAddress(pstrArgv[i++]);

	if (data_mask)
		data_mask = GetData(pstrArgv[i++]);

	data = GetData(pstrArgv[i++]);

	if (nArgc != i) {
		printf("program error\nneeded re-check %d %d\n", nArgc, i);
		return false;
	}

	if (stype == TERR)
		return Usage("type error");

	if (!MemInit())
		return Error("mem ctrl init error");

	pmctrl = GetMemCtrl(stype);
	if (!pmctrl) {
		Error("get mem ctrl error");
		goto error;
	}

	if (!addr) {
		Error("addr error");
		goto error;
	}
	if (addr & (pmctrl->nIncSize-1)) {
		Error("address alignment error");
		goto error;
	}

	/*
	 * CAUTION
	 *
	 * we can not use 32 bit shift.
	 * this operation is undefined in C !!
	 * So, this time we use 8bit shift and loop
	 */
	for (i = 0; i < pmctrl->nIncSize; i++)
		data_check = data_check << 8;

	if (data & data_check) {
		Error("input data error");
		goto error;
	}

	if (data_mask & data_check) {
		Error("data mask error");
		goto error;
	}

	if (data_mask) {
		u32 current;

		if (!pmctrl->fnRead(addr, &current))
			goto error;

		data = (current & ~data_mask) | (data & data_mask);
	}

	pmctrl->fnEdit(addr, &data);
	ret = true;

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
	if ((CMD_HIT(pstrArgv[0], "me.b") ||
	     CMD_HIT(pstrArgv[0], "me.w") ||
	     CMD_HIT(pstrArgv[0], "me.l") ||
	     IsCmdHit())
	    &&
	    ((nArgc == 3) || (nArgc == 5)))
		return true;

	return false;
}
INSTALL_CMD(me)
