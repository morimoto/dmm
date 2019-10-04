/************************************************************************


			dmm.h

2008/09  morimoto
************************************************************************/
#ifndef DMM_H
#define DMM_H

//======================================================================
//
//
//			include
//
//
//======================================================================
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

//======================================================================
//
//
//			define
//
//
//======================================================================
#define PRODUCE_BY      "Produce by        : Kuninori Morimoto"
#define SPECIAL_THANKS  "Special thanks to : Hideyuki Sano\n"

#ifdef _DEBUG
#define DMSG(args...)  printf(args)
#else
#define DMSG(args...)
#endif

//=====================================
//
//	type
//
//=====================================
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef int REGID;

//=====================================
//
//	color print
//
//=====================================
#define BLUE  printf("\x1b[34m")
#define CLAR  printf("\x1b[0m")

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define Error(msg) _Error(msg, __func__, __LINE__)

//======================================================================
//
//
//			function
//
//
//======================================================================
uintptr_t GetAddress(const char *);
enum ESTYPE GetStype(const char *);
u32  GetData(const char *);
bool _Error(const char *, const char *, int);

#include "memctrl.h"
#include "cmd.h"

#endif /* DMM_H */
