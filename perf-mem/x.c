#include <stdlib.h>
#include <unistd.h>

#define SIZE 10000000 /* 10 M */

// Expect: 'SIZE' stores
void f1(int* a, int a_len) {
  int* a_i = a;
  for (int i = 0; i < a_len; i++, a_i++) {
    *a_i = i;
  }
}


// Expect: 'SIZE - 1' loads
void f2(int* a, int a_len) {
  int* a_i = a;
  for(int i = 1; i < a_len; i++, a++) {
    *a_i  = *(a_i - 1);
  }
}


int main() {
  int* a = (int*)malloc(sizeof(int)*SIZE);

  f1(a, SIZE);
  f2(a, SIZE);
  
  free(a);
  return 0;
}
