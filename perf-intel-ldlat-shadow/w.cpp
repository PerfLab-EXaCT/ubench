#include <cstdlib>
#include <cstdlib>
#include <cmath>

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


__attribute__((noinline)) double f2(int cnt, int a, int b) // a >= 0 and b >= 0
{
  double a_ = a;
  double b_ = b;
  double x = a + b + 1; // x > 0
  for (int i = 0; i < cnt; i++) {
    x += 0.013 * a_ * x + 0.27 * b_;
  }

  return x; // res > 0
}


__attribute__((noinline)) node* f1(node* cur, int size, int cnt)
{
  double temp = 1.0;

  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < size; j++) {
      
      double x = f2(cnt, i, j);
      double x_frac = x - floor(x);

      if (x_frac <= 0.1) {
	temp = (j+i+1)/temp;
      }
      else if (0.1 < x_frac && x_frac <= 0.2) {
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

