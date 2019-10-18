TOP           = $(shell /bin/pwd)

-include $(TOP)/.config

all : memd.ko

obj-m := memd.o

memd.ko : memd.c ${TOP}/../version.h
	@echo "CC $@"
	@make -s -C $(KPATH) M=$(TOP) modules

clean :
	@make -s -C $(KPATH) -r M=$(TOP) clean

mrproper: clean
	@rm -f .config
