# -*-Mode: makefile;-*-
#****************************************************************************

CC = gcc -std=c99 -g -O0 -Wall

TGT = x

TEST = 4986 9973 19946 39891

#****************************************************************************

TEST_PERF_MEM = $(addprefix perf-mem-, $(TEST))

TEST_PERF_REP = $(addprefix perf-rep-, $(TEST))


#****************************************************************************

all : help

help :
	@echo "Targets"
	@echo "  test: run test"
	@echo "  mem:  collect perf mem data "
	@echo "  rep:  show perf reports"
	@echo "  clean, distclean"

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

.PHONY: mem rep

perf-stat stat :
	@if perf --version | egrep "version 2.6" >& /dev/null ; then \
	  perf stat -e cpu/mem-loads/pp ./$(TGT) ; \
	else \
	  perf stat -e cpu/mem-loads,ldlat=1/P ./$(TGT) ; \
	fi

# perf stat -e r003c ./x
# perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x


perf-mem-% : $(TGT)
	@echo "*** Perf-Mem trace, period $(*F) ***"
	@if perf --version | egrep "version 2.6" >& /dev/null ; then \
	  perf mem -t load record -c $(*F) -o $@ -- ./$(TGT) ; \
	else \
	  perf mem record -e list ; \
	  perf mem -t load record -e ldlat-loads -- ./$(TGT) ; \
	  mv perf.data $@ ; \
	fi

perf-rep-% : perf-mem-%
	perf     report --stdio -n -i $<
#	perf mem report --stdio -v -U -i $<

