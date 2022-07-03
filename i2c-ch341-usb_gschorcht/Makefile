MODULE_NAME=i2c-ch341-usb
MODULE_VERSION=1.0.0

define EOL


endef

DKMS       := $(shell which dkms)
PWD        := $(shell pwd) 
KVERSION   := $(shell uname -r)
KERNEL_DIR  = /lib/modules/$(KVERSION)/build
MODULE_DIR  = /lib/modules/$(KVERSION)

ifneq ($(DKMS),)
MODULE_INSTALLED := $(shell dkms status $(MODULE_NAME))
else
MODULE_INSTALLED =
endif

strip:= strip --strip-unneeded
MODULE_NAME  = i2c-ch341-usb
obj-m       := $(MODULE_NAME).o   

all:
	@echo ; echo 'try : ' ; echo '     $(foreach aa1,$(koS),rmmod $(aa1);) ; modprobe i2c-dev '
	@echo 'insmod ./$(ko00).ko '
	@echo 'insmod ./$(ko01) '
	@echo
	@echo 'i -> insert ( .ko module )'
	@echo 'r -> remove ( .ko module )'
	@echo


ko00:=i2c-ch341-usb
ko01:=$(ko00).$(KVERSION).ko
koS:=ch341_buses i2c_ch341_usb ch341

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
gs:
	git status

$(MODULE_NAME).ko: $(MODULE_NAME).c
	make -C $(KERNEL_DIR) M=$(PWD) modules


b build: $(MODULE_NAME).ko
	make -C $(KERNEL_DIR) M=$(PWD) modules
	test -f $(ko00).unstrip.ko || ( \
        /bin/cp -f $(ko00).ko $(ko00).unstrip.ko ; \
        $(strip) $(ko00).ko )
	test -f $(ko01) || cp $(ko00).ko $(ko01)


c clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f examples/gpio_input examples/gpio_output

ifeq ($(DKMS),)  # if DKMS is not installed

install: $(MODULE_NAME).ko
	cp $(MODULE_NAME).ko $(MODULE_DIR)/kernel/drivers/i2c/busses
	depmod
	
uninstall:
	rm -f $(MODULE_DIR)/kernel/drivers/i2c/busses/$(MODULE_NAME).ko
	depmod

else  # if DKMS is installed

install: $(MODULE_NAME).ko
ifneq ($(MODULE_INSTALLED),)
	@echo Module $(MODULE_NAME) is installed ... uninstall it first
	@make uninstall
endif
	@dkms install .
	
uninstall:
ifneq ($(MODULE_INSTALLED),)
	dkms remove -m $(MODULE_NAME) -v $(MODULE_VERSION) --all
	rm -rf /usr/src/$(MODULE_NAME)-$(MODULE_VERSION)
endif

endif  # ifeq ($(DKMS),)

h help:
	@echo ; echo "$${help01}"

m:
	vim Makefile

define help01
https://github.com/gschorcht/i2c-ch341-usb

endef
export help01
