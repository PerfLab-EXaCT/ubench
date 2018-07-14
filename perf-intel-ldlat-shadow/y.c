#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>

#define TYPE int
//#define TYPE long

size_t SIZE =  10000000; /* 10 M */
//size_t SIZE = 100000000; /* 100 M */

// Expect: 'SIZE' stores (+ frame loads)
void f1(TYPE* a, size_t a_len) {
  TYPE x = 1;
  for (size_t i = 0; i < a_len; i++) {
    x *= -1;
    a[i] = x;
  }
}


// Expect: 'SIZE - 2' memory loads (+ frame loads)
void f2(TYPE* a, TYPE* b, size_t len) {
  for (size_t i = 1; i < (len - 1); i++) {
    a[i] = b[i];
  }
}


int main() {
  TYPE* a = (TYPE*)malloc(sizeof(TYPE)*SIZE);
  TYPE* b = (TYPE*)malloc(sizeof(TYPE)*SIZE);

  f1(a, SIZE);
  f2(a, b, SIZE);

  //fprintf(stdout, "output: %ld, %ld\n", (long)a[SIZE - 1], (long)b[SIZE - 1]);
  free(a);
  free(b);
  return 0;
}
