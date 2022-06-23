
PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build

GccOpt:= -Wall -Werror -g -O3

strip:=	strip --strip-debug 
strip:=	strip --strip-unneeded

proj:=main_linux_i2c01
mid:=\
	i2c_bus_init \
	i2c_tx \
	i2c_reg_write
src01=$(proj).c $(foreach aa1,$(mid),$(aa1).c)
bin01=$(proj).$(KVERSION).bin

$(bin01): $(src01)
	@echo
	gcc $(GccOpt) -o $@ $^
	cp $@ $@.unstrip.bin
	$(strip) $@ 
	ls -l $@*

a all: $(bin01)
	@echo

c clean :
	@echo
	rm -f $(bin01)

r run :
	./$(bin01)

