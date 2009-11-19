CC            = $(CROSS_COMPILE)gcc
AR            = $(CROSS_COMPILE)ar
STRIP         = $(CROSS_COMPILE)strip
INCLUDE       = -I$(TOP)/include -I$(TOP) -I$(CPUDIR)
CFLAGS        = -Wall -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion $(EXTR)
MAKEFILE      = Makefile $(TOP)/include/config.mk
MAKEOPTION    = LFLAGS="$(LFLAGS)" CROSS_COMPILE="$(CROSS_COMPILE)" TOP="$(TOP)" INCLUDE="$(INCLUDE)"
DEPENDTGT     = $(OBJS:.o=.c) $(DEPEND)
ATARGET       = builtin.a
LIBS          = $(addsuffix /builtin.a, $(SUBDIR)) $(addsuffix /builtin.a, $(CPUDIR))
CPUDIR        = $(TOP)/cpu/none

.PHONY : $(SUBDIR) $(CPUDIR) autofile _clean

$(TARGET): depend $(CPUDIR) $(SUBDIR) $(LIBS) $(OBJS) $(MAKEFILE)
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(INCLUDE) $(LFLAGS) -o $@ $(OBJS) $(LIBS)
	@$(STRIP) $@

$(ATARGET): $(OBJS) $(AUTOFILES) $(MAKEFILE)
	@echo "AR $@"
	@$(AR) $(ARFLAGS) $@ $(OBJS)

$(SUBDIR) $(CPUDIR):
	@echo "CD $@"
	@make -s $(MAKEOPTION) -C $@ $(ATARGET)

.c.o:
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(INCLUDE) $(LFLAGS) -c $<

regs.h: regs
	@rm -f $@
	@./regs > $@

regs: regs.def
	@rm -f $@
	@gcc $(INCLUDE) $(TOP)/cpu/regdef.c -o $@

autofile:
ifneq ($(AUTOFILES),)
	make -s $(MAKEOPTION) $(AUTOFILES)
endif

_depend:
	@echo "create depend on `pwd`";
	@for f in $(DEPENDTGT); do \
		$(CC) -M $$f $(INCLUDE) >> depend.mk; \
	done

depend: 
	@rm -f depend.mk
	@for dir in $(CPUDIR) $(SUBDIR) ; do \
		make -s $(MAKEOPTION) -C $$dir autofile; \
	done
	@for dir in $(CPUDIR) $(SUBDIR) ; do \
		make -s $(MAKEOPTION) -C $$dir _depend; \
	done
	@: > depend

_clean:
	@rm -f depend.mk
	@rm -f depend
	@rm -f *.o $(TARGET) $(ATARGET) $(AUTOFILES) $(OBJS)

-include depend.mk