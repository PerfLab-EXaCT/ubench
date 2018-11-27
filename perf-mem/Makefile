# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = x

PERIOD = 4986 9973 19946 39891

PERIOD_DLA = 13973 # 9973 # 119730

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : help

help :
	@echo "Targets"
	@echo "  dla:   perf DLA test"
	@echo "  ldlat: perf ld-latency test"
	@echo "  stat:  basic perf stat test"
	@echo "  clean"
	@echo "  distclean"

$(TGT) : % : %.c
	$(CC) -o $@ $<

test : ldlat

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT)) \
	  $(TEST_PERF_LDLAT1_DAT) \
	  $(addsuffix .old, $(TEST_PERF_LDLAT1_DAT)) \
	  $(TEST_PERF_LDLAT2_DAT) \
	  $(addsuffix .old, $(TEST_PERF_LDLAT2_DAT))

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
	mem_uops_retired_all_loads_code \
	mem_inst_retired.all_loads\:upp \
	mem_inst_retired.all_loads \
	\
	mem_load_retired.l1_hit\:upp \
	mem_load_retired.l2_hit\:upp \
	mem_load_retired.fb_hit\:upp \
	mem_load_retired.l3_hit\:upp \
        mem_load_retired.l3_miss\:upp \
	\
	l2_rqsts.all_pf \
	l2_lines_in.all \
	l1d.replacement \
	offcore_response.all_pf_data_rd.any_response \
	offcore_response.pf_l1d_and_sw.any_response \
	offcore_response.pf_l2_data_rd.any_response \
	offcore_response.pf_l3_data_rd.any_response


TEST_PERF_DLA_DAT = $(addprefix perf-dla_dat-, $(DLA_EVENTS))
TEST_PERF_DLA_RPT = $(addprefix perf-dla_rpt-, $(DLA_EVENTS))


dla : dla_rpt

dla_dat : $(TEST_PERF_DLA_DAT)

dla_rpt : $(TEST_PERF_DLA_RPT)

.PHONY: dla dla_dat dla_rpt


perf-dla_dat-mem_uops_retired_all_loads_code : $(TGT)
	perf record -e cpu/event=0xd0,umask=0x81/upp -c $(PERIOD_DLA) -o $@ ./$(TGT)

perf-dla_dat-% : $(TGT)
	perf record -e $(*F) -c $(PERIOD_DLA) -o $@ ./$(TGT)

perf-dla_rpt-% : perf-dla_dat-%
	perf report --stdio $(PERF_RPT_OPT) -n -i $<


#----------------------------------------------------------------------------
# ldlat:
# - key problem: miss loads while monitoring a load
#----------------------------------------------------------------------------

TEST_PERF_LDLAT1_DAT = $(addprefix perf-ldlat1_dat-, $(PERIOD))

TEST_PERF_LDLAT2_DAT = $(addprefix perf-ldlat2_dat-, $(PERIOD))

TEST_PERF_LDLAT1_RPT = $(addprefix perf-ldlat1_rpt-, $(PERIOD))

TEST_PERF_LDLAT2_RPT = $(addprefix perf-ldlat2_rpt-, $(PERIOD))


ldlat : ldlat1_rpt ldlat2_rpt

ldlat1_dat : $(TEST_PERF_LDLAT1_DAT)

ldlat1_rpt : $(TEST_PERF_LDLAT1_RPT)

ldlat2_dat : $(TEST_PERF_LDLAT2_DAT)

ldlat2_rpt : $(TEST_PERF_LDLAT2_RPT)

.PHONY: ldlat ldlat1_dat ldlat1_rpt ldlat2_dat ldlat2_rpt


perf-ldlat1_dat-% : $(TGT)
	perf record -W -e cpu/mem-loads,ldlat=1/upp -c $(*F) -o $@ ./$(TGT)

# -W: sample by weight
# -d: record addresses

# perf record -W -d -e cpu/mem-loads,ldlat=4/upp -c $(*F)
# perf record -W -d -e cpu/event=0xcd,umask=0x1,ldlat=4/upp -c $(*F)
# perf record -W -d -e cpu/event=0xcd,umask=0x1,ldlat=4,period=1/upp

# old: perf record -e cpu/mem-loads,ldlat=1/P -c $(*F)

perf-ldlat2_dat-% : $(TGT)
	@echo "*** Perf-Mem trace, period $(*F) ***"
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; then \
	  perf mem -t load record -c $(*F) -o $@ -- ./$(TGT) ; \
	else \
	  perf mem -t load record -e ldlat-loads -- -o $@ ./$(TGT) ; \
	fi


perf-ldlat1_rpt-% : perf-ldlat1_dat-%
	perf report --stdio $(PERF_RPT_OPT) -n -i $<

perf-ldlat2_rpt-% : perf-ldlat2_dat-%
	perf report --stdio $(PERF_RPT_OPT) -n -i $<
#	perf mem report --stdio -v -U -i $<


#----------------------------------------------------------------------------
# stat test
#----------------------------------------------------------------------------

stat : perf-stat

.PHONY: stat


perf-stat : $(TGT)
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] >& /dev/null ; then \
	  perf stat -e cpu/mem-loads,ldlat=1/pp ./$(TGT) ; \
	  perf stat -e cpu/config=0x1cd,config1=0x3,config2=0x0/pp ./$(TGT) ; \
	else \
	  perf stat -e cpu/mem-loads/upp ./$(TGT) ; \
	  perf stat -e cpu/mem-loads,ldlat=1/upp ./$(TGT) ; \
	  perf stat -e cpu/event=0x0b,umask=0x10/upp ./$(TGT) ; \
	  perf stat -e cpu/config=0x1cd,config1=0x1f,config2=0x0/upp ./$(TGT) ; \
	fi

# perf stat -d -d -d <app>
