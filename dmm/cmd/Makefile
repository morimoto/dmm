OBJS_COMMON = cmd-help.o cmd-md.o cmd-me.o cmd-version.o

SRC_ALL     = $(OBJS_COMMON:.o=.c)

OBJS      = cmd.o $(OBJS_COMMON)
AUTOFILES = cmdlist.c cmdlist.h

include $(TOP)/include/config.mk

cmdlist.c: cmdlist.h $(TOP)/include/cmd.h
	@rm -f $@
	@echo "#include \"cmdlist.h\""                                        >> $@
	@echo "static STCMDLIST s_tblCmdList[] = {"                           >> $@
	@grep -h INSTALL_CMD $(SRC_ALL) | sed -e "s/INSTALL_CMD/CMD_DEFINE/g" >> $@
	@echo "};"                                                            >> $@
	@echo "CMD_LIST_SIZE;"                                                >> $@

cmdlist.h: $(SRC_ALL) $(TOP)/include/cmd.h
	@grep -h INSTALL_CMD $(SRC_ALL) | sed -e "s/INSTALL_CMD/CMD_EXTERN/g" > $@
