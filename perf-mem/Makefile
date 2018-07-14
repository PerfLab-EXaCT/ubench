# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = y

PERIOD = 4986 9973 19946 39891

PERIOD_DLA = 13973

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : help

help :
	@echo "Targets"
	@echo "  dla:   Intel DLA test"
	@echo "  ldlat: Intel load latency test"
	@echo "  clean"
	@echo "  distclean"

% : %.c
	$(CC) -o $@ $<

test : ldlat dla

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT)) \
	  $(TEST_PERF_LDLAT_DAT) \
	  $(addsuffix .old, $(TEST_PERF_LDLAT_DAT)) \

distclean : clean
	$(RM) $(TGT)


.PHONY: all help clean distclean test


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
	mem_inst_retired.all_loads\:upp

TEST_PERF_DLA_DAT = $(addprefix perf-dla_dat-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT = $(addprefix perf-dla_rpt-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT = $(addprefix perf-dla_annot-, $(DLA_EVENTS))

dla : dla_rpt dla_annot

dla_dat : $(TEST_PERF_DLA_DAT)

dla_rpt : $(TEST_PERF_DLA_RPT)

.PHONY: dla dla_dat dla_rpt


perf-dla_dat-mem_uops_retired_all_loads_code : $(TGT)
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD_DLA) -o $@ ./$(TGT)

perf-dla_dat-% : $(TGT)
	perf record -e $(*F) -c $(PERIOD_DLA) -o $@ ./$(TGT)

perf-dla_rpt-% : perf-dla_dat-%
	perf report --stdio $(PERF_RPT_OPT) -n -i $<

perf-dla_annot-% : perf-dla_dat-%
	perf annotate --stdio $(PERF_RPT_OPT) -n -i $<
#	FIXME: insert nops

#----------------------------------------------------------------------------
# ldlat:
#----------------------------------------------------------------------------

TEST_PERF_LDLAT_DAT = $(addprefix perf-ldlat_dat-, $(PERIOD))

TEST_PERF_LDLAT_RPT = $(addprefix perf-ldlat_rpt-, $(PERIOD))

ldlat : ldlat_rpt

ldlat_dat : $(TEST_PERF_LDLAT_DAT)

ldlat_rpt : $(TEST_PERF_LDLAT_RPT)

.PHONY: ldlat ldlat_dat ldlat_rpt


perf-ldlat_dat-% : $(TGT)
	perf record -W -e cpu/mem-loads,ldlat=1/upp -c $(*F) -o $@ ./$(TGT)
# 	-W: sample by weight
# 	-d: record addresses

perf-ldlat_rpt-% : perf-ldlat_dat-%
	perf report --stdio $(PERF_RPT_OPT) -n -i $<

