OBJS        = sh7366.o sh7722.o sh7723.o sh7724.o sh7786.o sh.o
DEPEND      = *.h
AUTOFILES   = cpulist.c regs.h regs

include $(TOP)/include/config.mk

cpulist.c:
	$(TOP)/cpu/cpulist.sh ./sh*.c
