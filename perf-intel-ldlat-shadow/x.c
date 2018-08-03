#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>

//#define TYPE int
#define TYPE long

size_t SIZE1 =  50000000; /* 50 M */
size_t SIZE2 = 100000000; /* 100 M */
//size_t SIZE = 10000000000; /* 10000 M */

__attribute__((noinline)) void shuffle(TYPE* array, size_t n)
{
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      TYPE t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}


// Expect: 'SIZE ' loads
__attribute__((noinline)) void f1(TYPE* a, TYPE* idx, size_t a_len) {
  for (size_t i = 0; i < a_len; i++) {
    TYPE idx_i = idx[i];
    if (idx_i % 2 == 0) {
      a[i] = a[idx_i];
    }
    else {
      a[i] = a[idx[idx_i]];
    }
  }
}


int main() {
  TYPE* a1 = (TYPE*)malloc(sizeof(TYPE)*SIZE1);
  TYPE* a2 = (TYPE*)malloc(sizeof(TYPE)*SIZE2);
  TYPE* idx1 = (TYPE*)malloc(sizeof(TYPE)*SIZE1);
  TYPE* idx2 = (TYPE*)malloc(sizeof(TYPE)*SIZE2);

  for (size_t i = 0; i < SIZE1; i++) {
    idx1[i] = rand() % SIZE1;
  }

  for (size_t i = 0; i < SIZE2; i++) {
    idx2[i] = i;
  }
  shuffle(idx2, SIZE2);

  f1(a1, idx1, SIZE1);
  f1(a2, idx2, SIZE2);
  f1(a1, idx1, SIZE1);

  free(a1);
  free(a2);
  free(idx1);
  free(idx2);
  return 0;
}
