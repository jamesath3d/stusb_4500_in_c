PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build

define EOL


endef
strip:= strip --strip-debug
strip:= strip --strip-unneeded


ko00:=ch341-buses
ko01:=$(ko00).$(KVERSION).ko
koS:=ch341_buses i2c_ch341_usb ch341


obj-m       := $(ko00).o

$(ko00)-objs := ch341-core.o ch341-i2c.o ch341-gpio.o ch341-spi.o

$(if $(wildcard $(KERNEL_DIR)/),,$(info directory $(KERNEL_DIR) don\'t exist.) \
	$(info apt install linux-headers-$(KVERSION))$(error ===exit===))

all:
	@echo ; echo 'try : ' ; echo '     $(foreach aa1,$(koS),rmmod $(aa1);) ; modprobe i2c-dev '
	@echo 'insmod ./$(ko00).ko '
	@echo 'insmod ./$(ko01) '
	@echo
	@echo 'i -> insert ( .ko module )'
	@echo 'r -> remove ( .ko module )'
	@echo


r remove:
	@echo ; 
	$(foreach aa1,$(koS),((lsmod |grep $(aa1)) && rmmod $(aa1) )|| echo>/dev/null$(EOL))
	@lsmod |grep -i ch341;echo>/dev/null
i insert: remove
	modprobe i2c-dev &> /dev/null
	insmod ./$(ko01)
	@echo
	@grep -i ch341 /sys/class/i2c-adapter/i2c-*/name
	@lsmod |grep -i ch341



m:
	vim Makefile

b build :
	make -C $(KERNEL_DIR) M=$(PWD) modules
	test -f $(ko00).unstrip.ko || ( \
		/bin/cp -f $(ko00).ko $(ko00).unstrip.ko ; \
		$(strip) $(ko00).ko )
	test -f $(ko01) || cp $(ko00).ko $(ko01)
c clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
h help:
	@echo ; echo "$${help01}"

define help01
https://github.com/frank-zago/ch341-i2c-spi-gpio

# The CH341 is declined in several flavors, and may support one or more of UART, SPI, I2C and GPIO, but not always simultaneously:
# 
# - CH341 A/B/F: UART, Printer, SPI, I2C and GPIO
# - CH341 C/T: UART and I2C
# - CH341 H: SPI
# They work in 3 different modes, with only one being presented depending on the USB PID:
# 
# - 0x5523: UART mode, covered by the USB `ch341` serial driver
# - 0x5512: SPI/I2C/GPIO mode, covered by this ch341_buses driver
# - 0x5584: Parallel printer mode, covered by the USB `usblp` driver
# From linux kernel 5.10 to 5.16, the 0x5512 PID was unfortunately also claimed by the driver for the UART part, and will conflict with this driver. Blacklisting that module or deleting it will solve that problem. In /etc/modprobe.d/blacklist.conf, add the following line to prevent loading of the serial driver:

blacklist ch341

groupadd ch341
adduser "$USER" ch341
echo 'SUBSYSTEMS=="usb" ATTRS{idProduct}=="5512" ATTRS{idVendor}=="1a86" GROUP="ch341" MODE="0660"' > /etc/udev/rules.d/99-ch341.rules

#### modprobe i2c-dev , or no "/dev/i2c-x" will be appears.
#### maybe , spi-bitbang.ko is needed.

udev rule debugging :
ACTION=="add", SUBSYSTEM=="gpio"      , RUN+="/bin/bash -c /test_udev.sh"
ACTION=="add", SUBSYSTEM=="i2c"       , RUN+="/bin/bash -c /test_udev.sh"
ACTION=="add", SUBSYSTEM=="spi_master", RUN+="/bin/bash -c /test_udev.sh"
ACTION=="add", SUBSYSTEM=="usb"       , RUN+="/bin/bash -c /test_udev.sh"

### test_udev.sh
echo "$0 ==== start" >> /tmp/log.001.test.4.${USEC_INITIALIZED}.txt
set                  >> /tmp/log.001.test.4.${USEC_INITIALIZED}.txt
echo "$0 ==== end"   >> /tmp/log.001.test.4.${USEC_INITIALIZED}.txt



endef
export help01
