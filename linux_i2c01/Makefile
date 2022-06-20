
PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build


proj:=linux_i2c01
src01=$(proj).c
bin01=$(proj).$(KVERSION).bin

$(bin01): $(src01)
	gcc -o $@ $^

a all: $(bin01)

c clean :
	rm -f $(bin01)


