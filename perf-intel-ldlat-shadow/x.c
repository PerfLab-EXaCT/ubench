#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>

#define TYPE int
//#define TYPE long

size_t SIZE1 =  10000000; /* 10 M */
size_t SIZE2 = 100000000; /* 100 M */
//size_t SIZE = 10000000000; /* 10000 M */

static void shuffle(int *array, size_t n)
{
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}


// Expect: 'SIZE ' loads
static void f1(TYPE* a, TYPE* idx, size_t a_len) {
  for (size_t i = 0; i < a_len; i++) {
    a[i]  = a[idx[i]];
  }
}

// Expect: 'SIZE ' loads
static void f2(TYPE* a, TYPE* b, size_t a_len) {
  for (size_t i = 0; i < a_len; i++) {
    a[i]  = b[i];
  }
}


int main() {
  TYPE* a = (TYPE*)malloc(sizeof(TYPE)*SIZE1);
  TYPE* b = (TYPE*)malloc(sizeof(TYPE)*SIZE2);
  TYPE* idx1 = (TYPE*)malloc(sizeof(TYPE)*SIZE1);
  TYPE* idx2 = (TYPE*)malloc(sizeof(TYPE)*SIZE2);

  for (size_t i = 0; i < SIZE1; i++) {
    idx1[i] = rand() % SIZE1;
  }

  for (size_t i = 0; i < SIZE2; i++) {
    idx2[i] = i;
  }
  shuffle(idx2, SIZE2);

  f1(a, idx1, SIZE1);
  f1(b, idx2, SIZE2);
  f2(a, b, SIZE1);
  f1(b, idx2, SIZE2);
  f2(a, b, SIZE1);

  free(a);
  free(b);
  free(idx1);
  free(idx2);
  return 0;
}
