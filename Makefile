TOP           = $(shell /bin/pwd)
TARGET        = dmm
SUBDIR        = cmd lib
OBJS          = main.o

-include $(TOP)/.config

.PHONY : clean
include ./include/config.mk

debug:
	make LFLAGS="-D_DEBUG -g" 

clean:
	@make -s $(MAKEOPTION) _clean
	@for dir in $(SUBDIR); do \
		make -s $(MAKEOPTION) -C $$dir _clean;\
	done
