/************************************************************************


			d2f

2023/07  Yoshihiro Shimoda <yoshihiro.shimoda.uh@renesas.com>
************************************************************************/
#include "dmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//=====================================
//
//	help
//
//=====================================
static STHELPMSG s_Help[] = {
	{
		"d2f", "dump memory to file",
		"dmm d2f[.d, .w, .l] [-t] addr [len]\n"
		"    -t    : Use 10 digits instead of 8 for filename\n"
		"    addr  : address\n"
		"    len   : length\n"
		" ex) dmm d2f.b 0x88000000 12\n"
		"     dmm d2f.w 0x88000000 0x20\n"
		"     dmm d2f   0x88000ff0 32    # it mean LONG"
	},
	HELPMSGEND
};

//=====================================
//
//	dump_to_file
//
//=====================================
static int dump_to_file(STMEMCTRL *pCtrl, uintptr_t ulAddr, u32 ulLen,
			const char *format)
{
	char filename[64];
	int ret = false;
	size_t written;
	uintptr_t i;
	FILE *fp;
	u32 val;

	DMSG("%s\n", __func__);

	sprintf(filename, format, ulAddr);

	fp = fopen(filename, "w+");
	if (!fp)
		return false;

	//----------------------
	// read memory and write file
	//----------------------
	for (i = (ulAddr & ~(uintptr_t)0xF); i < ulAddr + ulLen; i += pCtrl->nIncSize) {
		if (!pCtrl->fnRead(i, &val))
			goto out;

		written = fwrite(&val, 1, pCtrl->nIncSize, fp);
		if (written != pCtrl->nIncSize)
			goto out;
	}

	ret = true;

out:
	fclose(fp);

	return ret;
}

//=====================================
//
//	cmd
//
//=====================================
static bool cmd(int nArgc, char *pstrArgv[])
{
	enum ESTYPE stype = CmdGetDataSize(pstrArgv[0], TLONG);
	const char *format = "0x%08lx.bin";
	STMEMCTRL *pmctrl = NULL;
	bool ret = false;
	uintptr_t addr;
	int res;
	u32 len;

	while ((res = getopt(nArgc, pstrArgv, "t")) != -1) {
		switch (res) {
		case 't':
			format = "0x%010lx.bin";
			break;
		default:
			return Usage("unknown option");
		}
	}

	nArgc -= optind;
	pstrArgv += optind;

	if (nArgc != 2)
		return Usage("invalid numbers of arguments");

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

	len = GetData(pstrArgv[1]);

	ret = dump_to_file(pmctrl, addr, len, format);

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
	if (CMD_HIT(pstrArgv[0], "d2f.b") ||
	    CMD_HIT(pstrArgv[0], "d2f.w") ||
	    CMD_HIT(pstrArgv[0], "d2f.l") ||
	    IsCmdHit())
		return true;

	return false;
}
INSTALL_CMD(d2f)
