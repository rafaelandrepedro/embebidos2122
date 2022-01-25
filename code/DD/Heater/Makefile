obj-m := heater.o
heater-objs:=heatermodule.o utils.o
#KDIR := /home/bruno/buildroot/buildroot-cam/output/build/linux-custom
KDIR := /home/computador/buildroot/buildroot-2021.02.5/output/build/linux-custom
ARCH ?= arm
#CROSS_COMPILE ?= ~/buildroot/buildroot-cam/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-
CROSS_COMPILE ?= ~/buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-

export ARCH CROSS_COMPILE


KBUILD_TARGETS	:= clean help modules modules_install

.PHONY: all $(KBUILD_TARGETS)

all: modules
$(KBUILD_TARGETS):
	#$(MAKE) -C $(KDIR) M=$(shell pwd) $@ EXTRA_CFLAGS="$(MY_CFLAGS)"
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@
	
clean: 
	rm *.o *.ko *.mod
