/* Analyze improvements of avoiding cache pollution
 * Gather runtime for random accesses to a small array (fits in cache)
 *  with interleaved accesses to a large array (doesnt fit in cache)
 *  Avoid cache pollution from large array using PREFETCH_NTA
 *
 *  Algorithm:
 *  for N iterations (loop in main)
 *    // INVARIANT: for each inner loop, number of accesses is the "same"
 *    // small footprint + reuse (sum_arr_small)
 *    // Vary X : {4, 8, 64,256,1024}
 *      for X iterations
 *           random accesses of footprint S
 *
 *    // large footprint + no reuse (sum_arr_large)
 *    // INVARIANT X : 4
 *    foreach element of data structure D with footprint X * S
 *      // VARY prefetch-hint: {implicit hw, explicit, nta}
 *      // VARY b: {0, 32, 64} bytes
 *      prefetch-hint A + b
 *      access element address A
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "print_time.h"

#define TYPE double
#define sizeRatio 4
uint32_t PD = 8;

TYPE __attribute__ ((noinline)) add_elem( TYPE in1, TYPE in2)
{
	TYPE result;
	result = in1+in2;
	return result;
}

void  __attribute__ ((noinline)) init_elem ( uint64_t *ar1, uint64_t arCnt)
{
	 for(uint64_t i=0; i<arCnt; i++) {
		 *(ar1+i) = (random() % arCnt);
	 }
}

TYPE __attribute__ ((noinline)) sum_arr_small(TYPE *ar1, uint64_t *ar2, uint64_t arCnt)
{
	TYPE check_sum=0;
	uint64_t j=0;
	for (uint64_t k=0; k<sizeRatio*256; k++) {
		for(uint64_t i=0; i<arCnt; i++) {
			//j = (random() % arCnt); // overrides any improvement observed from PREFETCH_NTA
			j = *(ar2+i);
			check_sum += (*(ar1+j));
		}
	}
	return check_sum;
}
   
TYPE __attribute__ ((noinline)) sum_arr_large(TYPE *ar1, uint64_t arCnt)
{
  //printf("PD value %d\n", PD);
	TYPE check_sum=0;
  uint64_t i=0;

	#ifdef DO_PREFETCH_NTA_ADV0  
	  for(i=0; i<=arCnt-2; i+=2) {
	    __builtin_prefetch((ar1+i),0,0);
      check_sum+=  ((*(ar1+i))*(*(ar1+i+1)));
    }
    return check_sum;
  #endif
	#ifdef DO_PREFETCH_NTA  
	  __builtin_prefetch((ar1+PD),0,0);
  #endif
	#ifdef DO_PREFETCH_T0  
	  __builtin_prefetch((ar1+PD),0,3);
  #endif
  for (i=0; i<=PD-2; i+=2) {
    check_sum+= ((*(ar1+i)) * (*(ar1+i+1)));
  }
	for(i=PD; i<=arCnt-PD; i+=PD) {
  	#ifdef DO_PREFETCH_NTA  
	    __builtin_prefetch((ar1+i+PD),0,0);
    #endif
  	#ifdef DO_PREFETCH_T0  
	    __builtin_prefetch((ar1+i+PD),0,3);
    #endif
    for (uint64_t j =i; j<= i+PD-2; j+=2) {
  	  	check_sum += ((*(ar1+j))*(*(ar1+j+1)));
    }
	}
  
	return check_sum;
}

int main(void) {
	struct timespec start, finish;            
	char *str_log=(char *) malloc(500*sizeof(char)); 
  // L3 - Unified 32768K - 1/2 is 2*1024*1024 doubles
  uint64_t arSizeSmall = 2*1024*1024;
  int loopCnt = 600;
 
  // L1 - Data 32K - half is 2048 (8 bytes)
  //uint64_t arSizeSmall = 2048;
  //int loopCnt =  200000;
  uint64_t arSizeLarge = arSizeSmall*sizeRatio;
	
	sprintf(str_log, "Array Run time");  
	#ifdef DO_PREFETCH_NTA
	  strcat(str_log, " PREFETCH_NTA ");
	#endif
	#ifdef DO_PREFETCH_NTA_ADV0
	  strcat(str_log, " PREFETCH_NTA ADV - 0 ");
	#endif
	#ifdef DO_PREFETCH_T0
	  strcat(str_log, " PREFETCH_T0 ADV - 8");
	  PD = 8;
	#endif
	#ifdef PD_8
	  strcat(str_log, "ADV - 8");
	  PD = 8;
	#endif
	#ifdef PD_4
	  strcat(str_log, "ADV - 4");
	  PD = 4;
	#endif
	#ifdef PD_16
	  strcat(str_log, "ADV - 16");
	  PD = 16;
	#endif

	TYPE *ar_Small = malloc ((arSizeSmall)*sizeof(TYPE));
	uint64_t *ar_Small_Index = malloc ((arSizeSmall)*sizeof(uint64_t));
	init_elem(ar_Small_Index, arSizeSmall);
	TYPE *ar_Large = malloc ((arSizeLarge)*sizeof(TYPE));

	for (int i=0; i < (arSizeSmall); i++) {
	  *(ar_Small+i) = i+0.36235;
	}
	for (int i=0; i < (arSizeLarge); i++) {
	  *(ar_Large+i) = i+1+0.79717;
	}
	TYPE resSmall=0;
	TYPE resLarge=0;
	clock_gettime(CLOCK_REALTIME, &start); 
  for (int i=0; i< loopCnt; i++) {	
		//printf("i - %d \n");
		resSmall += sum_arr_small(ar_Small, ar_Small_Index, arSizeSmall);
		resLarge += sum_arr_large(ar_Large, arSizeLarge);
		//resSmall += sum_arr_small(ar_Small, ar_Small_Index, arSizeSmall);
	}
	clock_gettime(CLOCK_REALTIME, &finish); 
	printf("Small %lf ", resSmall);
	printf("Large %lf ", resLarge);
                                       
	print_time(str_log, start, finish);
  free(ar_Small);
  free(ar_Large);
  return 0;
}
