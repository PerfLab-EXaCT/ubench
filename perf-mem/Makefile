# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = x

TEST = 4986 9973 19946 39891

#****************************************************************************

TEST_PERF_MEM = $(addprefix perf-mem-, $(TEST))

TEST_PERF_REP = $(addprefix perf-rep-, $(TEST))


#****************************************************************************

SHELL = /bin/bash

PERF_VER := $(shell perf --version)

all : help

help :
	@echo "Targets"
	@echo "  test: run test: collect data and show reports."
	@echo "  mem:  collect perf mem data"
	@echo "  rep:  show perf reports"
	@echo "  stat, rec: other tests"
	@echo "  clean"
	@echo "  distclean"

$(TGT) : x.c
	$(CC) -o $@ $<

test : $(TGT) rep

clean :
	$(RM) $(TEST_PERF_MEM)

distclean : clean
	$(RM) x


.PHONY: all help clean distclean test

#****************************************************************************

mem : $(TEST_PERF_MEM)

rep : $(TEST_PERF_REP)

rec : perf-record

stat : perf-stat

.PHONY: mem rep stat rec


perf-record :
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; then \
	  : ; \
	else \
	  perf record -e cpu/mem-loads,ldlat=1/P -c 1 -o $@ ./$(TGT) ; \
	fi

.PHONY: perf-record


perf-stat :
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] >& /dev/null ; then \
	  perf stat -e cpu/mem-loads,ldlat=1/pp ./$(TGT) ; \
	  perf stat -e cpu/config=0x1cd,config1=0x3,config2=0x0/pp ./$(TGT) ; \
	else \
	  perf stat -e cpu/mem-loads/P ./$(TGT) ; \
	  perf stat -e cpu/mem-loads,ldlat=1/P ./$(TGT) ; \
	  perf stat -e cpu/config=0x1cd,config1=0x1f,config2=0x0/P ./$(TGT) ; \
	  perf stat -e cpu/event=0x0b,umask=0x10/P ./$(TGT) ; \
	fi


# perf stat -e r003c ./x
# perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x


perf-mem-% : $(TGT)
	@echo "*** Perf-Mem trace, period $(*F) ***"
	@if [[ "$(PERF_VER)" =~ "version 2.6" ]] ; then \
	  perf mem -t load record -c $(*F) -o $@ -- ./$(TGT) ; \
	else \
	  perf mem record -e list ; \
	  perf mem -t load record -e ldlat-loads -- ./$(TGT) ; \
	  mv perf.data $@ ; \
	fi

perf-rep-% : perf-mem-%
	perf     report --stdio -n -i $<
#	perf mem report --stdio -v -U -i $<

