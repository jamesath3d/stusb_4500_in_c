
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
	st45_38_analyze_nvm \
	st45_39_dump_config \
	\
	st45_3a_voltage_current_check \
	st45_3b_convert_config_to_nvm_buf \
	st45_3c_exit_test_mode \
	\
	st45_3d_nvm_enter_write \
	st45_3e_nvm_write_8_bytes \
	\
	st45_01_read_config \
	\
	i2c_11_bus_init \
	i2c_21_reg_read_write_test \
	i2c_23_reg_write \
	\
	i2c_31_tx 
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

e8:
	@echo; echo; echo; echo; echo; echo; echo; echo;
e16: e8
	@echo; echo; echo; echo; echo; echo; echo; echo;


c clean :
	@echo
	rm -f $(bin01)

#r run : $(bin01)
r run : e16 clean all run_only
ro run_only: 
	@echo
	@echo
	./$(bin01)

