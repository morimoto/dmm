OBJS_COMMON = cmd-common-help.o cmd-common-md.o cmd-common-me.o cmd-common-version.o
OBJS_SH     = cmd-sh-bsc.o  cmd-sh-cpu.o  cmd-sh-mstp.o   cmd-sh-pfc.o cmd-sh-bsc.o cmd-sh-io.o

SRC_ALL     = $(OBJS_COMMON:.o=.c)
SRC_SH      = $(OBJS_SH:.o=.c)

OBJS      = cmd.o $(OBJS_COMMON)
AUTOFILES = cmdlist.c cmdlist.h

ifeq ($(findstring sh,$(CPUDIR)), sh)
OBJS     += $(OBJS_SH)
SRC_ALL  += $(SRC_SH)
endif

include $(TOP)/include/config.mk

cmdlist.c: cmdlist.h
	@rm -f $@
	@echo "#include \"cmdlist.h\""                                        >> $@
	@echo "static STCMDLIST s_tblCmdList[] = {"                           >> $@
	@grep -h INSTALL_CMD $(SRC_ALL) | sed -e "s/INSTALL_CMD/CMD_DEFINE/g" >> $@
	@echo "};"                                                            >> $@
	@echo "CMD_LIST_SIZE;"                                                >> $@

cmdlist.h:
	@grep -h INSTALL_CMD $(SRC_ALL) | sed -e "s/INSTALL_CMD/CMD_EXTERN/g" > $@
