CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = -lm

all:	intel-prefetch-ctl

intel-prefetch-ctl:	intel-prefetch-ctl.o
	$(CC) $(LFLAGS) -o intel-prefetch-ctl intel-prefetch-ctl.o

intel-prefetch-ctl.o:	intel-prefetch-ctl.c
	$(CC) $(CFLAGS) -c intel-prefetch-ctl.c

clean:	
	rm -f *.o *~ intel-prefetch-ctl

