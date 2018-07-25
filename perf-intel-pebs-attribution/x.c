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


// Expect: 'SIZE' memory loads (+ frame loads)
TYPE f2(TYPE* a, size_t a_len) {
  TYPE z = 0;
  for (size_t i = 0; i < a_len; i++) {
    z += a[i]; // executed serially on -O0
  }
  return z;
}


int main() {
  TYPE* a = (TYPE*)malloc(sizeof(TYPE)*SIZE);

  f1(a, SIZE);
  f2(a, SIZE);

  //fprintf(stdout, "output: %ld, %ld\n", (long)a[SIZE - 1], (long)b[SIZE - 1]);
  free(a);
  return 0;
}
