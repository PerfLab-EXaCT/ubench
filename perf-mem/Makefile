# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = x

TGTy = y

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
	offcore_response.pf_l3_data_rd.any_response \



# These "non-precise" events can be easily throttled
#	mem_load_retired.l1_hit \
#	mem_load_retired.l2_hit \
#	mem_load_retired.fb_hit \
#	mem_load_retired.l3_hit \
#       mem_load_retired.l3_miss


# cycle_activity.stalls_l1d_miss

# mem_inst_retired.all_loads       MEM_UOPS_RETIRED.ALL_LOADS
# mem_inst_retired.lock_loads      MEM_UOPS_RETIRED.LOCK_LOADS
# mem_inst_retired.split_loads     MEM_UOPS_RETIRED.SPLIT_LOADS
# mem_inst_retired.stlb_miss_loads MEM_UOPS_RETIRED.STLB_MISS_LOADS

# mem_inst_retired.all_stores       MEM_UOPS_RETIRED.ALL_STORES
# mem_inst_retired.split_stores     MEM_UOPS_RETIRED.SPLIT_STORES
# mem_inst_retired.stlb_miss_stores MEM_UOPS_RETIRED.STLB_MISS_STORES

# mem_load_retired.l1_hit   MEM_LOAD_UOPS_RETIRED.L1_HIT
# mem_load_retired.l2_hit   MEM_LOAD_UOPS_RETIRED.L2_HIT
# mem_load_retired.l3_hit   MEM_LOAD_UOPS_RETIRED.L3_HIT
# mem_load_retired.fb_hit   MEM_LOAD_UOPS_RETIRED.HIT_LFB

# mem_load_retired.l1_miss  MEM_LOAD_UOPS_RETIRED.L1_MISS
# mem_load_retired.l2_miss  MEM_LOAD_UOPS_RETIRED.L2_MISS
# mem_load_retired.l3_miss  MEM_LOAD_UOPS_RETIRED.L3_MISS

# mem_load_l3_hit_retired.xsnp_hit  MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HIT
# mem_load_l3_hit_retired.xsnp_hitm MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HITM
# mem_load_l3_hit_retired.xsnp_miss MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_MISS
# mem_load_l3_hit_retired.xsnp_none MEM_LOAD_UOPS_LLC_HIT_RETIRED.XSNP_NONE

# mem_load_l3_miss_retired.local_dram  MEM_LOAD_UOPS_LLC_MISS_RETIRED.LOCAL_DRAM
# mem_load_l3_miss_retired.remote_dram MEM_LOAD_UOPS_LLC_MISS_RETIRED.REMOTE_DRAM

# ? l2_rqsts.pf_hit, l2_rqsts.pf_miss, load_hit_pre.sw_pf
# ? UOPS_RETIRED.ALL (if load or store is tagged)


# mem_uops_retired:all_loads [= mem_inst_retired.all_loads]
#   perf record -d -e cpu/event=0xd0,umask=0x81/upp -c 9973
#   perf record -d -e mem_inst_retired.all_loads -c 9973

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

#	perf script -F event,dso,ip,sym,symoff -i <input>
#	perf annotate -n --no-source --full-paths --stdio -i <input>
# 	perf report -n -v -U --no-demangle --stdio -i


# export-to-sqlite
# export-to-postgresql
# event_analyzing_sample

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
	  perf stat -e cpu/mem-loads/P ./$(TGT) ; \
	  perf stat -e cpu/mem-loads,ldlat=1/P ./$(TGT) ; \
	  perf stat -e cpu/config=0x1cd,config1=0x1f,config2=0x0/P ./$(TGT) ; \
	  perf stat -e cpu/event=0x0b,umask=0x10/P ./$(TGT) ; \
	fi

# perf stat -d -d -d <app>
