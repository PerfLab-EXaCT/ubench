# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = x

TEST = 4986 9973 19946 39891

#****************************************************************************

TEST_PERF_MEM = $(addprefix perf-mem-, $(TEST))

TEST_PERF_REC = $(addprefix perf-rec-, $(TEST))

TEST_PERF_MEMREP = $(addprefix perf-memrep-, $(TEST))

TEST_PERF_RECREP = $(addprefix perf-recrep-, $(TEST))


#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : help

help :
	@echo "Targets"
	@echo "  test: run test: collect data and show reports."
	@echo "  mem|rec:     show reports"
	@echo "  domem|dorec: collect data"
	@echo "  stat: other tests"
	@echo "  clean"
	@echo "  distclean"

$(TGT) : x.c
	$(CC) -o $@ $<

test : $(TGT) rep

clean :
	$(RM) $(TEST_PERF_MEM) $(TEST_PERF_REC)

distclean : clean
	$(RM) x


.PHONY: all help clean distclean test

#****************************************************************************

domem : $(TEST_PERF_MEM)

mem : $(TEST_PERF_MEMREP)

dorec : $(TEST_PERF_REC)

rec : $(TEST_PERF_RECREP)

stat : perf-stat

.PHONY: domem mem dorec rec stat


PERF_REP_OPT := $(shell \
	if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; \
	  then echo "" ; \
	else \
	  echo "--header" ; \
	fi )

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

perf-mem-% : $(TGT)
	@echo "*** Perf-Mem trace, period $(*F) ***"
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; then \
	  perf mem -t load record -c $(*F) -o $@ -- ./$(TGT) ; \
	else \
	  perf mem -t load record -e ldlat-loads -- ./$(TGT) ; \
	  mv perf.data $@ ; \
	fi

perf-rec-% : $(TGT)
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; then \
	  perf record -e cpu/mem-loads,ldlat=1/pp -c $(*F) -o $@ ./$(TGT) ; \
	else \
	  perf record -W -d -e cpu/event=0xcd,umask=0x1,ldlat=4/upp -c $(*F) -o $@ ./$(TGT) ; \
	fi


#	  perf record -W -d -e cpu/mem-loads,ldlat=4/upp -c $(*F) -o $@ ./$(TGT) ; \




#  perf record -W -d -e cpu/event=0xcd,umask=0x1,ldlat=4,period=1/upp \

#  perf record -e cpu/mem-loads,ldlat=1/P -c $(*F) -o $@ ./$(TGT) ; \


perf-memrep-% : perf-mem-%
	perf report --stdio $(PERF_REP_OPT) -n -i $<
#	perf mem report --stdio -v -U -i $<

perf-recrep-% : perf-rec-%
	perf report --stdio $(PERF_REP_OPT) -n -i $<

