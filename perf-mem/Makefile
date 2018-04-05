CC = gcc -std=c99 -g -O0 -Wall

TGT = x

TEST = perfmem-4986 perfmem-9973 perfmem-19946 perfmem-49865

$(TGT) : x.c
	$(CC) -o $@ $<

clean :
	$(RM) x

distclean : clean
	$(RM) perf.data*

test : $(TGT) $(TEST)

perfmem-% :
	@echo "*** Testing with period $(*F) ***"
	perf mem record -e list
	perf mem --type=load record --count $(*F) --output=perf-data-$(*F) -- ./x
#	perf report -n -i perf.data-$(*F)
# 	perf mem report
