#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>

#define TYPE int
//#define TYPE long

size_t SIZE =  10000000; /* 10 M */
//size_t SIZE = 100000000; /* 100 M */

// Expect: 'SIZE' stores
void f1(TYPE* a, size_t a_len) {
  TYPE* a_i = a;
  for (size_t i = 0; i < a_len; i++, a_i++) {
    *a_i = i;
  }
}


// Expect: 'SIZE - 1' loads
void f2(TYPE* a, size_t a_len) {
  TYPE* a_i = a; // 'a' is an array of size 'a_len'
  for (size_t i = 1; i < a_len; i++, a++) {
    *a_i = *(a_i - 1);
  }

  //fprintf(stdout, "f2 expect: %g loads\n", (double)(a_len - 1));
}


int main() {
  TYPE* a = (TYPE*)malloc(sizeof(TYPE)*SIZE);

  f1(a, SIZE);
  f2(a, SIZE);
  
  free(a);
  return 0;
}
