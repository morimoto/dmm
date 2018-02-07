TOP           = $(shell /bin/pwd)

-include $(TOP)/.config

all : memd.ko

obj-m := memd.o

memd.ko : memd.c
	make -C $(KPATH) -I$(KPATH)/include M=$(TOP) modules

clean :
	make -C $(KPATH) -r M=$(TOP) clean
	-rm -rf modules.order
