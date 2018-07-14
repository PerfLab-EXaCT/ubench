# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = y

PERIOD = 4986 9973 19946 39891

PERIOD_DLA = 13973 # 9973 # 119730

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : test

$(TGT) : % : %.c
	$(CC) -o $@ $<

test : dla ldlat

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT)) \
	  $(TEST_PERF_LDLAT_DAT) \
	  $(addsuffix .old, $(TEST_PERF_LDLAT_DAT)) \

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

PERF_OUT_REDIRECT := 2> /dev/null

GREP_ARGS := -e "\# Samples" -e "\# Event count"
# -e "\# event"

#----------------------------------------------------------------------------
# dla:
#----------------------------------------------------------------------------

DLA_EVENTS = \
	mem_inst_retired.all_loads\:upp

TEST_PERF_DLA_DAT = $(addprefix perf-dla_dat-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT = $(addprefix perf-dla_rpt-, $(DLA_EVENTS))

dla : dla_rpt

dla_dat : $(TEST_PERF_DLA_DAT)

dla_rpt : $(TEST_PERF_DLA_RPT)

.PHONY: dla dla_dat dla_rpt

perf-dla_dat-% : ./$(TGT)
	@perf record -e $(*F) -c $(PERIOD_DLA) -o $@ $< $(PERF_OUT_REDIRECT)

perf-dla_rpt-% : perf-dla_dat-%
	@echo "*** Load count: loads (precise) ***"
	@perf report --stdio $(PERF_RPT_OPT) -n -i $< \
	  | grep $(GREP_ARGS)

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
	@perf record -W -e cpu/mem-loads,ldlat=4/upp -c $(*F) -o $@ $< $(PERF_OUT_REDIRECT)
# 	-W: sample by weight
# 	-d: record addresses

perf-ldlat_rpt-% : perf-ldlat_dat-%
	@echo "*** Load count: ldlat / $(*F) ***"
	@perf report --stdio $(PERF_RPT_OPT) -n -i $< \
	  | grep $(GREP_ARGS)
