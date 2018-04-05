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
	perf mem -t load record -c $(*F) ./x
	mv perf.data perf.data-$(*F)
#	perf report -n -i perf.data-$(*F)


#  4986: 657
#  9973: 361, f1:60, f2:64
# 19946: 214,
# 49865: 125,
