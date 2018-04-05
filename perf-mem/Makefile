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

perf-mem-% : $(TGT)
	@echo "*** Testing with period $(*F) ***"
	@if perf --version | egrep "version 3." >& /dev/null; then \
	  perf mem record -e list ; \
	fi
	perf mem -t load record -c $(*F) -o $@ -- ./x

perf-rep-% : perf-mem-%
	perf     report --stdio -n -i $<
#	perf mem report --stdio -v -U -i $<
