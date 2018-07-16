# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = y

PERIOD = 13973 # 9973 # 119730

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : test

$(TGT) : % : %.c
	$(CC) -o $@ $<

test : dla

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT))

distclean : clean
	$(RM) $(TGT)


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
TEST_PERF_DLA_RPT = $(addprefix perf-dla_rpt-, $(DLA_EVENTS))

dla : dla_rpt

dla_dat : $(TEST_PERF_DLA_DAT)

dla_rpt : $(TEST_PERF_DLA_RPT)

.PHONY: dla dla_dat dla_rpt


perf-dla_dat-mem_uops_retired_all_loads_code : $(TGT)
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD) -o $@ $<

perf-dla_dat-% : $(TGT)
	perf record -e $(*F) -c $(PERIOD) -o $@ $<

perf-dla_rpt-% : perf-dla_dat-%
#	perf report --stdio $(PERF_RPT_OPT) -n -i $<
	perf annotate -s f1 -n --stdio -i $<
	perf annotate -s f2 -n --stdio -i $<
