
#KPATH=/media/hdd2/home/h-yokoya/proj/bsp/renesas-backport
KPATH=/media/hdd2/home/h-yokoya/proj/gen3/kernel/gitlab/rcar-bsp-linux

all : memd.ko

obj-m := memd.o

memd.ko : memd.c
	make -C $(KPATH) -I$(KPATH)/include M=`pwd` modules

clean :
	make -C $(KPATH) -r M=`pwd` clean
	-rm -rf modules.order
