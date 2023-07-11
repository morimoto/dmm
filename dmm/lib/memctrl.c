/*=============================================

		memctrl.c

2009/04  morimoto merge mem.c
2008/09  morimoto
==============================================*/
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "dmm.h"

#define MAX_MEMARRARY  5       // hack me!!
#define DEV_NAME  "/dev/mem"

static int s_nLastAddrMap = 0;
static STADDRMAP s_tblAddrMap[MAX_MEMARRARY];

static int	 s_nFd;
static STMEMCTRL s_Bmem;
static STMEMCTRL s_Wmem;
static STMEMCTRL s_Lmem;

//======================================================================
//
//
//			local function
//
//
//======================================================================
//=====================================
//
//	addrmaplinit
//
//=====================================
static bool addrmapinit(void)
{
	int i;

	for (i = 0; i < MAX_MEMARRARY; i++) {
		s_tblAddrMap[i].nSize = 0;
		s_tblAddrMap[i].rMap  = 0;
		s_tblAddrMap[i].rTgt  = 0;
	}

	s_nFd = open(DEV_NAME, O_RDWR);
	if (s_nFd < 0) {
		perror(DEV_NAME " open error !!\n"
		       "please use this command on root\n");
		return false;
	}

	return true;
}

//=====================================
//
//	IsInsideMap
//
//=====================================
static bool isinsidemap(const uintptr_t rAddr,
			STADDRMAP *pMap, STMEMCTRL *pCtrl)
{
	if (!pMap || !pCtrl)
		return Error("pMap or pCtrl is NULL");

	if (!pMap->rMap)
		return Error("ther is no orig");

	if ((pMap->rTgt			<= rAddr) &&
	    (pMap->rTgt + pMap->nSize	>= rAddr + pCtrl->nIncSize))
		return true;

	return false;
}

//=====================================
//
//	getaddrmap
//
//=====================================
static STADDRMAP *getaddrmap(uintptr_t rAddr, STMEMCTRL *pCtrl)
{
	int i = 0;
	u32 page_size = 0;
	uintptr_t tgt_addr = 0;
	STADDRMAP *pmap = NULL;

	//----------------------
	// check if there is addr map
	//----------------------
	for (i = 0; i < s_nLastAddrMap; i++) {
		pmap = s_tblAddrMap+i;
		if (isinsidemap(rAddr, pmap, pCtrl))
			return pmap;
	}

	//----------------------
	// size over
	//----------------------
	if (s_nLastAddrMap >= MAX_MEMARRARY) {
		printf("mmap error\n");
		return NULL;
	}

	//----------------------
	// create new one
	//----------------------
	page_size	= (u32)sysconf(_SC_PAGE_SIZE);
	tgt_addr	= rAddr & ~(uintptr_t)(page_size - 1);
	pmap		= s_tblAddrMap + s_nLastAddrMap;

	DMSG("create new mmap\n");
	DMSG("addr      : %p\n", (void *)rAddr);
	DMSG("tgt_addr  : %p\n", (void *)tgt_addr);
	DMSG("page_size : 0x%08x\n", page_size);

	pmap->rMap = (uintptr_t)mmap(NULL, page_size,
				     PROT_WRITE | PROT_READ,
				     MAP_SHARED,
				     s_nFd,
				     (off_t)tgt_addr);
	if ((void *)pmap->rMap == MAP_FAILED) {
		pmap->rMap = 0;
		perror("mmap error\n");
		return NULL;
	}

	pmap->nSize = page_size;
	pmap->rTgt  = tgt_addr;

	s_nLastAddrMap++;

	return pmap;
}

//=====================================
//
//	tgt2map
//
//=====================================
static uintptr_t tgt2map(STADDRMAP *pMap, const uintptr_t rAddr)
{
	return (rAddr - pMap->rTgt) + pMap->rMap;
}

//=====================================
//
//	bread
//
//=====================================
static bool bread(const uintptr_t rAddr, u32 *pRet)
{
	u8 *paddr = NULL;
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Bmem);

	if (!pmap)
		return Error("can not get pmap\n");

	paddr = (u8 *)tgt2map(pmap, rAddr);
	*pRet = (u32)*paddr;

	return true;
}

//=====================================
//
//	wread
//
//=====================================
static bool wread(const uintptr_t rAddr, u32 *pRet)
{
	u16 *paddr = NULL;
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Wmem);

	if (!pmap)
		return Error("can not get pmap\n");

	paddr = (u16 *)tgt2map(pmap, rAddr);
	*pRet = (u32)*paddr;

	return true;
}

//=====================================
//
//	lread
//
//=====================================
static bool lread(const uintptr_t rAddr, u32 *pRet)
{
	u32 *paddr = NULL;
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Lmem);

	if (!pmap)
		return Error("can not get pmap\n");

	paddr = (u32 *)tgt2map(pmap, rAddr);
	*pRet = (u32)*paddr;

	return true;
}

//=====================================
//
//	bdump
//
//=====================================
static bool bdump(const uintptr_t rAddr)
{
	u32 val;

	if (!bread(rAddr, &val))
		return false;

	printf("%02x", (u8)val);
	return true;
}

//=====================================
//
//	wdump
//
//=====================================
static bool wdump(const uintptr_t rAddr)
{
	u32 val;

	if (!wread(rAddr, &val))
		return false;

	printf("%04x", (u16)val);
	return true;
}

//=====================================
//
//	ldump
//
//=====================================
static bool ldump(const uintptr_t rAddr)
{
	u32 val;

	if (!lread(rAddr, &val))
		return false;

	printf("%08x", val);
	return true;
}

//=====================================
//
//	bedit
//
//=====================================
static bool bedit(const uintptr_t rAddr, void *pData)
{
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Bmem);
	u8 *paddr = (u8 *)tgt2map(pmap, rAddr);
	u8  data  = *((u8 *)pData);

	if (!pmap)
		return Error("can not get pmap\n");


	*paddr = data;

	return true;
}

//=====================================
//
//	wedit
//
//=====================================
static bool wedit(const uintptr_t rAddr, void *pData)
{
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Wmem);
	u16 *paddr = (u16 *)tgt2map(pmap, rAddr);
	u16  data  = *((u16 *)pData);

	if (!pmap)
		return Error("can not get pmap\n");

	*paddr = data;

	return true;
}

//=====================================
//
//	ledit
//
//=====================================
static bool ledit(const uintptr_t rAddr, void *pData)
{
	STADDRMAP *pmap = getaddrmap(rAddr, &s_Lmem);
	u32 *paddr = (u32 *)tgt2map(pmap, rAddr);
	u32  data  = *((u32 *)pData);

	if (!pmap)
		return Error("can not get pmap\n");

	*paddr = data;

	return true;
}

//=====================================
//
//	memctrlinit
//
//=====================================
static bool memctrlinit(void)
{
	s_Bmem.fnEdit   = bedit;
	s_Bmem.fnRead   = bread;
	s_Bmem.fnDump   = bdump;
	s_Bmem.nIncSize = 1;
	s_Bmem.EStype = TBYTE;

	s_Wmem.fnEdit   = wedit;
	s_Wmem.fnRead   = wread;
	s_Wmem.fnDump   = wdump;
	s_Wmem.nIncSize = 2;
	s_Wmem.EStype = TWORD;

	s_Lmem.fnEdit   = ledit;
	s_Lmem.fnRead   = lread;
	s_Lmem.fnDump   = ldump;
	s_Lmem.nIncSize = 4;
	s_Lmem.EStype = TLONG;

	return true;
}

//======================================================================
//
//
//			global function
//
//
//======================================================================

//=====================================
//
//	GetMemCtrl
//
//=====================================
STMEMCTRL *GetMemCtrl(enum ESTYPE EStype)
{
	switch (EStype) {
	case TBYTE: return &s_Bmem;
	case TWORD: return &s_Wmem;
	case TLONG: return &s_Lmem;
	case TERR:
	default:
		printf("error in %s\n", __func__);
	}

	return NULL;
}

//=====================================
//
//	MemExit
//
//=====================================
void MemExit(void)
{
	int i;

	for (i = 0; i < s_nLastAddrMap; i++) {
		if (s_tblAddrMap[i].rMap) {
			if (-1 == munmap((void *)s_tblAddrMap[i].rMap,
					 s_tblAddrMap[i].nSize))
				perror("munmap");

			s_tblAddrMap[i].nSize = 0;
			s_tblAddrMap[i].rMap  = 0;
			s_tblAddrMap[i].rTgt  = 0;
		}
	}

	close(s_nFd);
}

//=====================================
//
//	MemInit
//
//=====================================
bool MemInit(void)
{
	if (!addrmapinit())
		return false;

	if (!memctrlinit())
		return false;

	return true;
}
