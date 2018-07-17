# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall
ACC = gcc -c 

TGT1 = y
TGT2 = \
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

$(TGT2) : % : %.s
	$(CC) -o $@ $<

$(TGT1) : % : %.c
	$(CC) -o $@ $<

test : dla

clean :
	$(RM) \
	  $(TEST_PERF_DLA1_DAT) $(TEST_PERF_DLA2_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA1_DAT) $(TEST_PERF_DLA2_DAT)) \
	  $(TEST_PERF_DLA1_RPT) $(TEST_PERF_DLA2_RPT)

distclean : clean
	$(RM) $(TGT1) $(TGT2)


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


TEST_PERF_DLA1_DAT = $(addprefix perf-dla1_dat-, $(TGT1) $(TGT2))
TEST_PERF_DLA2_DAT = $(addprefix perf-dla2_dat-, $(TGT1) $(TGT2))
TEST_PERF_DLA1_RPT = $(addsuffix .txt, $(addprefix perf-dla1_rpt-, $(TGT1) $(TGT2)))
TEST_PERF_DLA2_RPT = $(addsuffix .txt, $(addprefix perf-dla2_rpt-, $(TGT1) $(TGT2)))

dla : dla_dat dla_rpt

dla_dat : $(TEST_PERF_DLA1_DAT) $(TEST_PERF_DLA2_DAT)
dla_rpt : $(TEST_PERF_DLA1_RPT) $(TEST_PERF_DLA2_RPT)

.PHONY: dla dla_dat dla_rpt

perf-dla1_dat-% : %
	perf record -e mem_inst_retired.all_loads:upp -c $(PERIOD) -o $@ $<

perf-dla2_dat-% : %
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@ $<

perf-dla1_rpt-%.txt : perf-dla1_dat-%
	perf annotate -s f1 -n --stdio -i $< > $@
	perf annotate -s f2 -n --stdio -i $< >> $@

perf-dla2_rpt-%.txt : perf-dla2_dat-%
	perf annotate -s f1 -n --stdio -i $< > $@
	perf annotate -s f2 -n --stdio -i $< >> $@


# DLA_EVENTS = \
# 	mem_uops_retired_all_loads_code \
# 	mem_inst_retired.all_loads\:upp \

# perf-dla_dat_asm-mem_uops_retired_all_loads_code : $(TGT2) 
# 	@$(foreach asm,$(ASM),perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@_$(asm) $(asm);)

# perf-dla_dat_asm-% : $(TGT2)
# 	@$(foreach asm,$(TGT2),perf record -e $(*F) -c $(PERIOD) -o $@_$(asm) $(asm);)

# perf-dla_rpt_asm-% : perf-dla_dat_asm-% $(TGT2)
# #	perf report --stdio $(PERF_RPT_OPT) -n -i $<
# 	@$(foreach asm,$(TGT2),perf annotate -s f1 -n --stdio -i $<_$(asm) >$<_$(asm).txt;)
# 	@$(foreach asm,$(TGT2),perf annotate -s f2 -n --stdio -i $<_$(asm) >>$<_$(asm).txt;)


