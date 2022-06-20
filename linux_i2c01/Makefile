
PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build


proj:=main_linux_i2c01
mid:=\
	i2c_tx \
	i2c_reg_write
src01=$(proj).c $(foreach aa1,$(mid),$(aa1).c)
bin01=$(proj).$(KVERSION).bin

$(bin01): $(src01)
	gcc -o $@ $^

a all: $(bin01)

c clean :
	rm -f $(bin01)


