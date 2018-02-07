TOP           = $(shell /bin/pwd)

-include $(TOP)/.config

all : memd.ko

obj-m := memd.o

memd.ko : memd.c
	@echo "CC $@"
	@make -s -C $(KPATH) -I$(KPATH)/include M=$(TOP) modules

clean :
	@make -s -C $(KPATH) -r M=$(TOP) clean
	@-rm -rf modules.order

mrproper: clean
	@rm -f .config
