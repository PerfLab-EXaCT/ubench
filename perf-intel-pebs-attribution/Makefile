# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall
ACC = gcc -c 

TGT = y
ASM = \
   y-1nop \
   y-2nop \
   y-3nop \
   y-4nop \
   y-5nop \
   y-6nop \
   y-7nop \
   y-8nop \

PERIOD = 13973 # 9973 # 119730

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : test

$(ASM) : % : %.s
	$(ACC) $< -o $@.out
	$(CC) -o $@ $@.out
	$(dla_rpt_asm)

$(TGT) : % : %.c
	$(CC) -o $@ $<

test : dla

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(TEST_PERF_DLA_DAT_ASM) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT)) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT_ASM))

distclean : clean
	$(RM) $(TGT) $(ASM)


.PHONY: all clean distclean test


#****************************************************************************
#****************************************************************************

PERF_RPT_OPT := $(shell \
	if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; \
	  then echo "" ; \
	else \
	  echo "--header" ; \
	fi )

#----------------------------------------------------------------------------
# dla:
#----------------------------------------------------------------------------

DLA_EVENTS = \
	mem_uops_retired_all_loads_code \
	mem_inst_retired.all_loads\:upp \

TEST_PERF_DLA_DAT = $(addprefix perf-dla_dat-, $(DLA_EVENTS))
TEST_PERF_DLA_DAT_ASM = $(addprefix perf-dla_dat_asm-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT = $(addprefix perf-dla_rpt-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT_ASM = $(addprefix perf-dla_rpt_asm-, $(DLA_EVENTS))

dla : dla_rpt dla_rpt_asm

dla_dat : $(TEST_PERF_DLA_DAT)
dla_dat_asm : $(TEST_PERF_DLA_DAT_ASM)

dla_rpt : $(TEST_PERF_DLA_RPT)
dla_rpt_asm : $(TEST_PERF_DLA_RPT_ASM)

.PHONY: dla dla_dat dla_rpt dla_dat_asm dla_rpt_asm

perf-dla_dat_asm-mem_uops_retired_all_loads_code : $(ASM) 
	@$(foreach asm,$(ASM),perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@_$(asm) $(asm);)

perf-dla_dat-mem_uops_retired_all_loads_code : $(TGT) 
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@ $<

perf-dla_dat_asm-% : $(ASM)
	@$(foreach asm,$(ASM),perf record -e $(*F) -c $(PERIOD) -o $@_$(asm) $(asm);)

perf-dla_dat-% : $(TGT)
	perf record -e $(*F) -c $(PERIOD) -o $@ $<

perf-dla_rpt_asm-% : perf-dla_dat_asm-% $(ASM)
#	perf report --stdio $(PERF_RPT_OPT) -n -i $<
	@$(foreach asm,$(ASM),perf annotate -s f1 -n --stdio -i $<_$(asm) >$<_$(asm).txt;)
	@$(foreach asm,$(ASM),perf annotate -s f2 -n --stdio -i $<_$(asm) >>$<_$(asm).txt;)

perf-dla_rpt-% : perf-dla_dat-%
#	perf report --stdio $(PERF_RPT_OPT) -n -i $<
	perf annotate -s f1 -n --stdio -i $< >$@.txt
	perf annotate -s f2 -n --stdio -i $< >>$@.txt
