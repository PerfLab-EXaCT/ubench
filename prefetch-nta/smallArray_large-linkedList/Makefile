SOURCE = array_link_list.c 
CC = gcc
CFLAGS  = -g -Wall -O3

.PHONY: all
all : arr_ll arr_nta arr_t0

arr_ll: $(SOURCE) 
	$(CC) $(CFLAGS) $(SOURCE) -o arr_ll
arr_nta: $(SOURCE)
	$(CC) $(CFLAGS) -DDO_PREFETCH_NTA $(SOURCE) -o arr_ll_p_nta 
arr_t0: $(SOURCE)
	$(CC) $(CFLAGS) -DDO_PREFETCH_T0 $(SOURCE) -o arr_ll_p_t0  


clean:
	rm ./arr_ll 
	rm ./arr_ll_p_nta 
	rm ./arr_ll_p_t0 
