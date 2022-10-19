Analyze improvements of avoiding cache pollution using microbenchmark

*  Gather runtime for repeated random accesses to a small array S (fits in cache) , 
	with interleaved accesses to a large data structure D (doesnt fit in cache) , 
	avoid cache pollution from large data structure D using PREFETCH_NTA

*  Uses PREFETCH_NTA instruction by specifying  _builtin_prefetch compiler directive 

*  Algorithm:
```
  for N iterations 
    // INVARIANT: for each inner loop, number of accesses is the "same"
    // small footprint + reuse 
    // Vary X : {4,8,16,32}
      for X iterations
           random accesses of footprint S

    // large footprint + no reuse 
    // VARY D: {strided, linked list of small arrays}
    // INVARIANT X
    foreach element of data structure D with footprint X * S
      // VARY prefetch-hint: {implicit hw, explicit, nta}
      // VARY b: {0, 32, 64} bytes
      prefetch-hint A + b
      access element address A
```




