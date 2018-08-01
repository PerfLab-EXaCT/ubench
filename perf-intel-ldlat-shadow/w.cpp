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
  node* next;
  double val;
};


int test1(int a,int b)
{
  int res = rand();
  if (res % 2 == 0){
    res = res +10;
  }

  res = res * a;

  if (a > b){
    res = a;
  }
  else{
    res =b;
  }
  res +=1;
  return res;
}


node* randomAccess3(node* cur,int size,int cnt)
{
  double temp =1.0;

  for(int i=0;i<cnt;i++){
    for(int j =0;j<size;j++){
      int val = rand()%8;
      test1(i,j);
      if(val<2){
	temp = (j+i+1)/temp;
      }
      else 
	if(val>=2 && val <6){
	  cur=cur->next;
	}
	else{
	  cur=cur->next;
	  cur=cur->next;
	  cur=cur->next;
	}
      cur=cur->next;
    }

    for(int j = 0; j <100;j++){
      cur=cur->next;
    }
  }

  return cur;
}


int main(int argc, const char** argv)
{
  int size = 1000; // atol(argv[1])/sizeof(node);
  int cnt = 1000;  // atoi(argv[2]);
  int numCall = 10;
  //if (argc > 3) {
  //  numCall = atoi(argv[3]);
  //}

  //cout<<"cache size: "<<size*sizeof(node)<<" array size: "<<size<<" numCall: "<<numCall<<endl;

  vector<node*> array;
  for(int i=0;i<size;i++){
    array.push_back(new node());
  }

  //for (int i=0;i<10;i++){
  //cout<<array[i]<<" ";
  //}
  //cout<<endl;

  random_shuffle(array.begin(),array.end());

  // for (int i=0;i<10;i++){
  //   cout<<array[i]<<" ";
  // }
  // cout<<endl;

  for (int i=0;i<size-1;i++){
    array[i]->next=array[i+1];
    array[i]->val = double(rand()%2000000)/1000000.0;
  }
  array[size-1]->next=array[0];
  array[size-1]->val = double(rand()%2000000)/1000000.0;

  node* cur = array[0];
	
  for(int i =0; i< numCall;i++){
    auto t1 = chrono::steady_clock::now();
    cur = randomAccess3(cur,size,cnt);
    auto t2 = chrono::steady_clock::now();
    auto diff = t2-t1;

    // s1+=chrono::duration<double>(diff).count();

  }

  // cout<<endl;
  // cout<<"time: "<<s1<<" "<<v1<<endl;
  // cout<<"time: "<<s2<<" "<<v2<<endl;
  // cout<<"time: "<<s3<<" "<<v3<<" "<<v4<<endl;

  for(int i=0;i<size;i+=size/32) {
    //cout<<array[i]->next<<",";
    if (size<32){
      i++;
    }
  }
  //cout <<endl;

  for(size_t i=0;i<array.size();i++){
    delete array[i];
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  //std::cout<<"done"<<std::endl;
}

