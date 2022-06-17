
projName:=stusb_4500_in_c
dateX1:=$(shell LC_ALL=C date +%Y_%m%d_%H%M%P_%S )

ifeq ($(USER),root)
	$(info )
	$(info "$0 can't run by $(USER). exit." )
	$(info )
	$(error exit )
endif

define EOL


endef

define callFUNC
$1 :
	$($1)
endef

all:

u up:
	git push -u origin main

gs git-status :
	git status

ga git-add:
	git add .

gc git-commit:
	git commit -a -m "$(dateX1)"

m:
	vim Makefile ; echo
v:
	vim Makefile.env ; echo

vp : vim_prepare
	Makefile:=$(shell test -L Makefile && realpath --relative-to=. Makefile || echo Makefile)
vim_prepare :
	rm -f tags \
		cscope.in.out \
		cscope.out \
		cscope.po.out
	mkdir -p _vim/
	echo $(Makefile)                            > _vim/file01.txt
	echo Makefile.env                          >> _vim/file01.txt
	echo $(vim_tags_objS) |xargs -n 1          >> _vim/file01.txt
	cscope -q -R -b -i                            _vim/file01.txt
	ctags -L                                      _vim/file01.txt

c01:=$(foreach aa1,$(wildcard                 src0?/ $(projName)/ ) ,$(shell find $(aa1) -name "*.c"))
h01:=$(foreach aa1,$(wildcard  xl_tt?/ xh_tt?/  h0?/ $(projName)/ ) ,$(shell find $(aa1) -name "*.h"))
o01:=$(foreach aa1,$(wildcard                   o0?/ $(projName)/ ) ,$(shell find $(aa1) -name makefile -o -name Makefile -o -name "*.mk"))

c09:=$(c01)
h09:=$(h01)
o09:=$(o01)

$(if $(c09),,$(error "c09 don't exit. check <projName> and run again. Exit. 83491981831 "))

vim_tags_objS:=$(shell echo . $(c09) $(h09) $(o09)|xargs -n 1 realpath --relative-to=.|sort -u)
vim_edit_objS1:=$(shell echo . $(c09)              |xargs -n 1 realpath --relative-to=.|sort -u)
export vim_edit_objS1
vim_edit_objS:=$(shell \
	echo "${vim_edit_objS1}" |xargs -n 1 |grep    /main.c$$ ; \
	echo "${vim_edit_objS1}" |xargs -n 1 |grep -v /main.c$$ \
	)

