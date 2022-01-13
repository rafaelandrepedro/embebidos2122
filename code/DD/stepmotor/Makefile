obj-m := stepmotor.o
stepmotor-objs:=stepmotormodule.o utils.o
KDIR := /home/bruno/buildroot/buildroot-cam2/output/build/linux-custom
ARCH ?= arm
CROSS_COMPILE ?= ~/buildroot/buildroot-cam2/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-

export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
	
clean: 
	rm *.o *.ko *.mod
