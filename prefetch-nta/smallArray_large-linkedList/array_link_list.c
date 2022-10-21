/* Analyze improvements of avoiding cache pollution
 * Gather runtime for random accesses to a small array (fits in cache)
 *  with interleaved accesses to a large array (doesnt fit in cache)
 *  Avoid cache pollution from large array using PREFETCH_NTA
 *
 *  Algorithm:
 *  fp_ratio = large_fp / small_fp
 *  for N iterations (loop in main)
 *    // INVARIANT: for random accesses of S, number of accesses is the "same"
 *    // small footprint + reuse (sum_arr_small)
 *    // Vary X : {4,32, 512, 1024}
 *    for (X * fp_ratio) iterations
 *           random accesses of footprint S
 *
 *    // Vary fp_ratio : {1, 1/2}
 *    // large footprint + no reuse (sum_list)
 *    foreach element of linked list D with footprint fp_ratio * S * 8 (each element is 64 bytes - cache line width)
 *      // VARY prefetch-hint: {implicit hw, explicit, nta}
 *      // b: {64} bytes
 *      prefetch-hint A + b
 *      access element address A
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "print_time.h"

#define TYPE double
#define sizeRatio 2

struct LLNode {
	TYPE val_A[2];
	TYPE val_B[2];
	TYPE val_C[2];
	uint8_t fill[8];
	struct LLNode* link;
};

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
	for (uint64_t k=0; k<sizeRatio*512; k++) {
		for(uint64_t i=0; i<arCnt; i++) {
			j = *(ar2+i);
			check_sum += (*(ar1+j));
		}
	}
	return check_sum;
}
   
TYPE __attribute__ ((noinline)) sum_list(struct LLNode* list_ptr)
{
	struct LLNode* cur_ptr=list_ptr;
	struct LLNode* nxt_ptr=list_ptr->link;
	TYPE flSum =0.0;
	uint8_t intSum=0;
	TYPE result=0.0;
	while(cur_ptr != NULL)
	{
		flSum =0.0;
		intSum=0;
	  #ifdef DO_PREFETCH_NTA
      if(nxt_ptr != NULL)
    		__builtin_prefetch(nxt_ptr,0,0);
  	#endif
	  #ifdef DO_PREFETCH_T0
      if(nxt_ptr != NULL)
    		__builtin_prefetch(nxt_ptr,0,3);
  	#endif
		for( int i=0; i<2; i++)
		{
			//printf( " pointer %p  A %lf B %lf C %lf result %lf \n", cur_ptr, cur_ptr->val_A[i], cur_ptr->val_B[i], cur_ptr->val_C[i], result);
			flSum += ((cur_ptr->val_A[i] * cur_ptr->val_B[i] ) * cur_ptr->val_C[i]);
		}
		for( int i=0; i<8; i++)
		{
			intSum += (cur_ptr->fill[i] );
		}
		result += flSum*(double)intSum;
		cur_ptr = cur_ptr->link;
		if(cur_ptr != NULL)
			nxt_ptr = cur_ptr->link;	
	}
	return result;
}

struct LLNode* __attribute__ ((noinline)) create_list(struct LLNode* cur_ptr, uint64_t count) {
struct LLNode* end_ptr;
	for (int k=0; k< count; k++) {
		for (int i=0; i<2; i++) 
		{
				cur_ptr->val_A[i] = k+0.24526;
				cur_ptr->val_B[i] = (k%2) +0.73226;
				cur_ptr->val_C[i] = (k%9) +0.98743;
		}
		for (int i=0; i<8; i++) 
		{
			cur_ptr->fill[i]=(k+i)%8; 
		}
		if( k != (count-1)) {
			struct LLNode* new_node = malloc(sizeof (struct LLNode));
			cur_ptr->link = new_node;
			cur_ptr = new_node;
		}
		else 
			cur_ptr->link = NULL;
	}
	end_ptr = cur_ptr;
	//printf(" return pointer %p \n", end_ptr);
	return end_ptr;
}

int main(void) {
	struct timespec start, finish;            
	char *str_log=(char *) malloc(500*sizeof(char)); 

	// L3 Junction - Unified 32768K - 1/2 is 2*1024*1024 doubles
  //uint64_t arSizeSmall = 2*1024*1024;
  // L3 - Unified Bluesky 19712K - 1/2 is 1232*1024 doubles
  uint64_t arSizeSmall = 1232*1024;

	uint64_t arSizeLarge = arSizeSmall;
	int listSize =arSizeLarge;

	struct LLNode* first = malloc(sizeof (struct LLNode));
	struct LLNode* one_ptr;
	struct LLNode* two_ptr;
	struct LLNode* three_ptr;
  // Runtime better  with NTA
 	//one_ptr= create_list(first , listSize); 

  // Runtime better  with NTA
 	//one_ptr= create_list(first , listSize/2); 
	//two_ptr = create_list(one_ptr, listSize/2); 

 	one_ptr= create_list(first , listSize/4); 

	sprintf(str_log, "Array Run time");  
	#ifdef DO_PREFETCH_NTA
	  strcat(str_log, " PREFETCH_NTA ");
	#endif
	#ifdef DO_PREFETCH_T0
	  strcat(str_log, " PREFETCH_T0 ");
	#endif
	TYPE *ar_Small = malloc ((arSizeSmall)*sizeof(TYPE));
	for (int i=0; i < (arSizeSmall); i++) {
	  *(ar_Small+i) = i+0.36235;
	}
	two_ptr = create_list(one_ptr, listSize/4); 

  uint64_t *ar_Small_Index = malloc ((arSizeSmall)*sizeof(uint64_t));
  init_elem(ar_Small_Index, arSizeSmall);
	/*TYPE *ar_Large = malloc ((arSizeLarge)*sizeof(TYPE));
	for (int i=0; i < (arSizeLarge); i++) {
	  *(ar_Large+i) = i+1+0.79717;
	}*/
	three_ptr = create_list(two_ptr, listSize/2); 

	TYPE resSmall=0;
	TYPE resLargeList=0;
	TYPE resLargeArray=0;
	//resLargeArray = sum_arr_large(ar_Large, arSizeLarge);
	clock_gettime(CLOCK_REALTIME, &start); 
  for (int i=0; i< 400; i++) {	
		resSmall += sum_arr_small(ar_Small, ar_Small_Index, arSizeSmall);
		resLargeList += sum_list(first);
	}
	clock_gettime(CLOCK_REALTIME, &finish); 
	printf("Small %lf ", resSmall);
	printf("Large %lf ", resLargeArray);
	printf("Large list %lf ", resLargeList);
                                       
	print_time(str_log, start, finish);
      	free(ar_Small);
      	//free(ar_Large);
      	return 0;
}
