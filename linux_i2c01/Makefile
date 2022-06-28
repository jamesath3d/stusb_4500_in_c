
PWD         := $(shell pwd)
KVERSION    := $(shell uname -r)
KERNEL_DIR  ?= /lib/modules/$(KVERSION)/build

GccOpt:= -Wall -Werror -g -O3

strip:=	strip --strip-debug 
strip:=	strip --strip-unneeded

proj:=main_linux_i2c01
mid:=\
	st45_31_nvm_read \
	st45_32_nvm_enter_read \
	st45_33_nvm_cmp \
	st45_34_read_top \
	\
	st45_35_nvm_write \
	st45_36_gen_new \
	\
	st45_37_dump_buf \
	\
	st45_01_read_config \
	\
	i2c_bus_init \
	i2c_tx \
	i2c_reg_read_write_test \
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

r run : $(bin01)
	@echo
	./$(bin01)

