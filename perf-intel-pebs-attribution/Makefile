# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall
ACC = gcc -c 

TGT1 = \
	x
#	y

TGT2 = \
   x-0nop \
   x-1nop \
   x-2nop \
   x-3nop \
   x-4nop \
   x-5nop \
   x-6nop \
   x-7nop \
   x-8nop

#   y-0nop \
#   y-1nop \
#   y-2nop \
#   y-3nop \
#   y-4nop \
#   y-5nop \
#   y-6nop \
#   y-7nop \
#   y-8nop

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

GREP_ARGS := -v -e "0 :.*nop"

PERF_VALIDATE = perf report -D -i $@ | grep -v -q --color THROT

dla : dla_dat dla_rpt

dla_dat : $(TEST_PERF_DLA1_DAT) $(TEST_PERF_DLA2_DAT)
dla_rpt : $(TEST_PERF_DLA1_RPT) $(TEST_PERF_DLA2_RPT)

.PHONY: dla dla_dat dla_rpt

perf-dla1_dat-% : %
	perf record -e mem_inst_retired.all_loads:upp -c $(PERIOD) -o $@ $<
	$(PERF_VALIDATE)

perf-dla2_dat-% : %
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@ $<
	$(PERF_VALIDATE)

define _rpt_template
  perf-dla$(1)_rpt-%.txt : perf-dla$(1)_dat-%
	perf annotate -s f1 -n --no-source --stdio -i $$< \
	  | grep $(GREP_ARGS) > $$@
	perf annotate -s f2 -n --no-source --stdio -i $$< \
	  | grep $(GREP_ARGS) >> $$@
endef

# N.B.: Beware of whitespace in $(call)!
$(foreach x, 1 2, $(eval $(call _rpt_template,$(x))))
