/*=============================================

                     migo-r

2008/11  morimoto
==============================================*/
/*
#include "dmm.h"
#include "cpu.h"
#include "cpu/sh/regs.h"
#include "cmd/cmdlist.h"
*/
#include "stdio.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef struct sh_addrmap {
    u32   nBitWidth;
    const char    *pName;
    u32  nAddr;
    u32  nSize;
}SHADDRMAP;

typedef struct sh_area {
    const char     *pArea;
    SHADDRMAP      *pMap;
    int             nMapSize;
}SHAREA;

typedef struct sh_board {
    const char  *pCPUName;
    SHAREA      *pSTArea;
    int         nAreaSize;
}SHBOARD;


SHADDRMAP area0_map = {
    .nBitWidth = 16,
    .pName     = "Nor Flash",
    .nAddr     = 0x00000000,
    .nSize     = 0x04000000
};

SHADDRMAP area1_map = {
    .nBitWidth = 16,
    .pName     = "SH 7722 Register",
    .nAddr     = 0x04000000,
    .nSize     = 0x04000000
};

SHADDRMAP area2_map = {
    .nBitWidth = 64,
    .pName     = "SDRAM",
    .nAddr     = 0x08000000,
    .nSize     = 0x04000000
};
SHADDRMAP area3_map = {
    .nBitWidth = 64,
    .pName     = "SDRAM",
    .nAddr     = 0x0C000000,
    .nSize     = 0x04000000
};
SHADDRMAP area4_map = {
    .nBitWidth = 16,
    .pName     = "Debug SMSC91C111",
    .nAddr     = 0x10000000,
    .nSize     = 0x04000000
};
SHADDRMAP area5a_map = {
    .nBitWidth = 16,
    .pName     = "USB controller",
    .nAddr     = 0x14000000,
    .nSize     = 0x02000000
};
SHADDRMAP area6a_map = {
    .nBitWidth = 16,
    .pName     = "NAND Flash",
    .nAddr     = 0x18000000,
    .nSize     = 0x02000000
};
SHADDRMAP area6b_map = {
    .nBitWidth = 16,
    .pName     = "Debug expansion bus",
    .nAddr     = 0x1A000000,
    .nSize     = 0x02000000
};

SHAREA area[] = {
    {
        .pArea    = "0",
        .pMap     = &area0_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "1",
        .pMap     = &area1_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "2",
        .pMap     = &area2_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "3",
        .pMap     = &area3_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "4",
        .pMap     = &area4_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "5a",
        .pMap     = &area5a_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "6a",
        .pMap     = &area6a_map,
        .nMapSize = 1,
    },
    {
        .pArea    = "6b",
        .pMap     = &area6b_map,
        .nMapSize = 1,
    },
};

SHBOARD migor = {
    .pCPUName = "SH 7722",
    .pSTArea   = area,
    .nAreaSize = ARRAY_SIZE(area),
};


void main( void )
{
    int i, j;

    printf( "CPU : %s\n", migor.pCPUName );
    for ( i=0 ; i<migor.nAreaSize ; i++ ) {
        printf("Area %s\n" , migor.pSTArea[i].pArea );
        for ( j=0 ; j<migor.pSTArea[i].nMapSize ; j++ ) {
            printf("  %d bits : 0x%08x - 0x%08x : %s\n",
                   migor.pSTArea[i].pMap[j].nBitWidth,
                   migor.pSTArea[i].pMap[j].nAddr,
                   migor.pSTArea[i].pMap[j].nAddr + migor.pSTArea[i].pMap[j].nSize -1,
                   migor.pSTArea[i].pMap[j].pName);
        }
    }
}
