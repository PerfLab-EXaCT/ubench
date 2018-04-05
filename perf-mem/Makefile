CC = gcc -std=c99 -g -O0 -Wall

TEST = perfmem-4986 perfmem-9973 perfmem-19946 perfmem-49865

x : x.c
	$(CC) -o $@ $<

clean :
	$(RM) x

distclean : clean
	$(RM) perf.data*

test : $(TEST)

perfmem-% :
	@echo "*** Testing with period $(*F) ***"
	perf mem record -e list
	perf mem --type=load record --count $(*F) -- ./x
	mv perf.data perf.data-$(*F)
#	perf report -n -i perf.data-$(*F)
# 	perf mem report
