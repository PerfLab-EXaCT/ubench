#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>


using namespace std;

struct node {
  node(double _val, node* _next = NULL) : next(_next) { }
  double val[100];
  node* next;
};


__attribute__((noinline)) int f2(int a, int b) // a >= 0 and b >= 0
{
  static int foo = -1;

  int res = foo * -1;
  if (res < 0) {
    res = -res + 10;
  }
  // res > 0

  res = res * a;

  if (a > b) {
    res = a;
  }
  else {
    res = b;
  }
  res += 1;
  return res; // res > 0
}


__attribute__((noinline)) node* f1(node* cur, int size, int cnt)
{
  double temp = 1.0;

  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < size; j++) {

      int x = f2(i, j) % 8;

      if (x < 2) {
	temp = (j+i+1)/temp;
      }
      else if (x >= 2 && x < 6) {
	cur=cur->next;
      }
      else {
	cur=cur->next;
	cur=cur->next;
	cur=cur->next;
      }
      cur=cur->next;
    }

    for(int j = 0; j < 100; j++){
      cur = cur->next;
    }
  }

  return cur;
}

extern double z;
double z;

int main(int argc, const char** argv)
{
  int size = 1000000; // atol(argv[1])/sizeof(node);
  int cnt = 100;  // atoi(argv[2]);
  int numCall = 5;
  //if (argc > 3) {
  //  numCall = atoi(argv[3]);
  //}

  // Create circular list
  vector<node*> array;
  for(int i = 0; i < size; i++){
    array.push_back(new node(i));
  }

  for (int i = 0; i < size - 1; i++) {
    array[i]->next = array[i+1];
  }
  array[size-1]->next = array[0];

  // Shuffle list
  std::random_shuffle(array.begin(), array.end());

  // Walk shuffled list
  node* cur = array[0];
  for (int i = 0; i < numCall; i++) {
    cur = f1(cur, size, cnt);
  }

  z = cur->val[0]; // prevent dead code elimination
  
  for (size_t i = 0; i < array.size(); i++) {
    delete array[i];
  }

  return 0;
}

