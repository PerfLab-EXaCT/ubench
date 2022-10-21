Analyze improvements of avoiding cache pollution using microbenchmark
-----------------------------------------------------------------------------

*  Gather runtime for repeated random accesses to a small array S (fits in cache) , 
	with interleaved accesses to a large data structure D (doesnt fit in cache) , 
	avoid cache pollution from large data structure D using PREFETCHNTA

*  Uses PREFETCH_NTA instruction
   https://stackoverflow.com/questions/48994494/how-to-properly-use-prefetch-instructions/48995540?noredirect=1#comment84996972_48994494

   * x86
     https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
     https://scripts.mit.edu/~birge/blog/accelerating-code-using-gccs-prefetch-extension/

   * ARM PLD,L3,STRM
     https://developer.arm.com/documentation/den0024/a/The-A64-instruction-set/Memory-access-instructions/Prefetching-memory


   * x86 example with Intel:
     https://github.com/Exa-Graph/grappolo/blob/develop/Utility/utilityClusteringFunctions.cpp

    ```
    ./Utility/utilityClusteringFunctions.cpp:74:#pragma noprefetch vDegree
    ./Utility/utilityClusteringFunctions.cpp:75:#pragma noprefetch vtxPtr
    ./Utility/utilityClusteringFunctions.cpp:76:#pragma prefetch vtxInd:3  # nta
    ./Utility/utilityClusteringFunctions.cpp:77:#pragma prefetch cInfo:3   # nta
    ./Utility/utilityClusteringFunctions.cpp:193:#pragma prefetch vtxInd:3 # nta
    ```

   * x86 example with GCC:
    ```
    prefetcht0      [rdi]    #   __builtin_prefetch(p,0,3);
    prefetcht1      [rdi]    #   __builtin_prefetch(p,0,2);
    prefetcht2      [rdi]    #   __builtin_prefetch(p,0,1);
    prefetchnta     [rdi]    #   __builtin_prefetch(p,0,0); - this one
    ```

* Algorithm (NT):
  ```
  for N iterations

    // Goal: accesses to small footprint >> access to large footprint
    
    fp_ratio = large_fp / small_fp

    // small footprint + reuse
    for (X * fp_ratio) iterations
      random accesses of footprint S

    // large footprint + no reuse
    // VARY D: {strided, linked list of small arrays}
    foreach element of data structure D with footprint X * S
      // VARY prefetch-hint: {implicit hw, explicit, nta}
      // VARY b: {0, 32, 64} bytes
      prefetch-hint A + b
      access element address A
  ```

*  Algorithm (YS):
```
  fp_ratio = large_fp / small_fp

  for N iterations 
    // INVARIANT: for random accesses of S, number of accesses is the "same"
    // small footprint + reuse 
    // Vary X : {4,32, 512, 1024}
    for (X * fp_ratio) iterations
           random accesses of footprint S

    // large footprint + no reuse 
    // VARY D: {strided, linked list of small arrays}
    foreach element of data structure D with footprint (fp_ratio * S)
      // VARY prefetch-hint: {implicit hw, explicit, nta}
      // VARY b: {16, 32, 64} bytes
      prefetch-hint A + b
      access element address A
```




