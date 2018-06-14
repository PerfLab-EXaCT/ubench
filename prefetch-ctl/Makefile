CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = $(CFLAGS)
LD_ADD = -lm

all:	intel-prefetch-ctl

intel-prefetch-ctl:	intel-prefetch-ctl.o
	$(CC) $(LFLAGS) -o intel-prefetch-ctl intel-prefetch-ctl.o $(LD_ADD)

intel-prefetch-ctl.o:	intel-prefetch-ctl.c
	$(CC) $(CFLAGS) -c intel-prefetch-ctl.c

clean:	
	rm -f *.o *~ intel-prefetch-ctl

