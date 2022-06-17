PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build

obj-m       := ch341-buses.o

ch341-buses-objs := ch341-core.o ch341-i2c.o ch341-gpio.o ch341-spi.o

$(if $(wildcard $(KERNEL_DIR)/),,$(info directory $(KERNEL_DIR) don\'t exist.) \
	$(info apt install linux-headers-$(KVERSION))$(error ===exit===))

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
	test -f ch341-buses.unstrip.ko || ( \
		/bin/cp -f ch341-buses.ko ch341-buses.unstrip.ko ; \
		strip --strip-debug ch341-buses.ko )
	@echo ; echo 'try : insmod ./ch341-buses.ko ' ; echo
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
