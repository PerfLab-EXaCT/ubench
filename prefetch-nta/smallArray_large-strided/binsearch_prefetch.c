#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_time.h"

int binarySearch(int *array, int number_of_elements, int key) {
	int low = 0, high = number_of_elements-1, mid;
	while(low <= high) {
		mid = (low + high)/2;
#ifdef DO_PREFETCH
		__builtin_prefetch (&array[(mid + 1 + high)/2], 0, 0);
		__builtin_prefetch (&array[(low + mid - 1)/2], 0, 0);
#endif

		if(array[mid] < key)
			 low = mid + 1;
		else if(array[mid] == key)
			 return mid;
		else if(array[mid] > key)
			 high = mid-1;
	}
	return -1;
}
int main() {
struct timespec start, finish, build;
char *str_log=(char *) malloc(50*sizeof(char));  
	int SIZE = 1024*1024*512;
	int *array =  malloc(SIZE*sizeof(int));
	for (int i=0;i<SIZE;i++){
		array[i] = i;
	}
	int NUM_LOOKUPS = 1024*1024*8;
	srand(time(NULL));
	int *lookups = malloc(NUM_LOOKUPS * sizeof(int));
  clock_gettime(CLOCK_REALTIME, &start);
	for (int i=0;i<NUM_LOOKUPS;i++){
		lookups[i] = rand() % SIZE;
	}
	for (int i=0;i<NUM_LOOKUPS;i++){
		int result = binarySearch(array, SIZE, lookups[i]);
	}
  clock_gettime(CLOCK_REALTIME, &finish);
    sprintf(str_log, "Bin Run time");
  #ifdef DO_PREFETCH
    strcat(str_log, " PREFETCH");
  #endif
    print_time(str_log, start, finish);
	free(array);
	free(lookups);
}

