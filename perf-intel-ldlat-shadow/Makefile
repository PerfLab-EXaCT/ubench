# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall
CXX = g++ -std=c++11 -g -O3 -Wall
#CXX = icpc -std=c++11 -g -Ofast -Wall

C_TGT = x y

CXX_TGT = w

TGTS = $(CXX_TGT) $(C_TGT)

PERIOD = 2313 4986 9973 19946 39891

PERIOD_DLA = 13973 # 9973 # 119730

#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : test

$(C_TGT) : % : %.c
	$(CC) -o $@ $<

$(CXX_TGT) : % : %.cpp
	$(CXX) -o $@ $<

test : dla ldlat

clean :
	$(RM) \
	  $(TEST_PERF_DLA_DAT) \
	  $(addsuffix .old, $(TEST_PERF_DLA_DAT)) \
	  $(TEST_PERF_LDLAT_DAT) \
	  $(addsuffix .old, $(TEST_PERF_LDLAT_DAT)) \
	  $(TEST_PERF_DLA_RPT) $(TEST_PERF_LDLAT_RPT)

distclean : clean
	$(RM) $(TGTS)


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
# --color
# -e "\# event"

PERF_VALIDATE  = perf report -D -i $@ | grep -v -q --color THROT
PERF_VALIDATEx = perf report -D -i $$@ | grep -v -q --color THROT


#----------------------------------------------------------------------------
# dla:
#----------------------------------------------------------------------------

TEST_PERF_DLA_DAT := $(addprefix perf-dla_dat-, $(TGTS))
TEST_PERF_DLA_RPT := $(addsuffix .txt, $(addprefix perf-dla_rpt-, $(TGTS)))

dla : dla_rpt

dla_dat : $(TEST_PERF_DLA_DAT)

dla_rpt : $(TEST_PERF_DLA_RPT)

.PHONY: dla dla_dat dla_rpt

perf-dla_dat-% : %
	@perf record -e mem_inst_retired.all_loads:upp -c $(PERIOD_DLA) -o $@ $< $(PERF_OUT_REDIRECT)
	@$(PERF_VALIDATE)

perf-dla_rpt-%.txt : perf-dla_dat-%
	@echo "*** precise load count: $(*F) / $(PERIOD_DLA) ***"
	@perf report --stdio $(PERF_RPT_OPT) -n -i $< > $@
	@grep $(GREP_ARGS) $@

#----------------------------------------------------------------------------
# ldlat:
#----------------------------------------------------------------------------

TGT_PRD_PAIRS := $(foreach tgt, $(TGTS), $(addprefix $(tgt)-, $(PERIOD)))
#TGT_PRD_PAIRS := $(foreach tgt, $(TGTS), $(foreach prd, $(PERIOD), $(tgt)-$(prd)))

TEST_PERF_LDLAT_DAT := $(addprefix perf-ldlat_dat-, $(TGT_PRD_PAIRS))
TEST_PERF_LDLAT_RPT := $(addsuffix .txt, $(addprefix perf-ldlat_rpt-, $(TGT_PRD_PAIRS)))

ldlat : ldlat_rpt

ldlat_dat : $(TEST_PERF_LDLAT_DAT)

ldlat_rpt : $(TEST_PERF_LDLAT_RPT)

.PHONY: ldlat ldlat_dat ldlat_rpt


define _ldlat_dat_template
  perf-ldlat_dat-%-$(1) : %
	@perf record -W -e cpu/mem-loads,ldlat=0/upp -c $(1) -o $$@ $$< $(PERF_OUT_REDIRECT)
	@$(PERF_VALIDATEx)
# 	-W: sample by weight
# 	-d: record addresses
endef

define _ldlat_rpt_template
  perf-ldlat_rpt-%-$(1).txt : perf-ldlat_dat-%-$(1)
	@echo "*** ldlat load count: $$(*F) / $(1) ***"
	@perf report --stdio $(PERF_RPT_OPT) -n -i $$< > $$@
	@grep $(GREP_ARGS) $$@
endef

# N.B.: Beware of whitespace in $(call)!
$(foreach x, $(PERIOD), $(eval $(call _ldlat_dat_template,$(x))))

$(foreach x, $(PERIOD), $(eval $(call _ldlat_rpt_template,$(x))))
