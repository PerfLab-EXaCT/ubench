

Expectation:
------------------------------------------------------------

Expect loop in f1 to have 70M loads (10M iterations; 7 load instructions).
Expect loop in f2 to have 80M loads (10M iterations; 8 load instructions).

Expect each load to have _715_ samples. (Each loop has 10M iterations;
sample period is 13973.)


Comments on attribution to function f2:
------------------------------------------------------------

* 0 nop: uneven distribution; attribution to jump (jb)

* 1 nop: six ops have ~800 samples; two have ~400
* 2 nop: four ops have ~1400 samples; four have <100
* 3 nop: three ops have  >1000 samples; three have ~300; one has ~800; one has ~100    
* 4 nop: four ops have ~1400 samples; four have <100
* 5 nop: four ops have ~1400 samples; four have <100
* 6 nops: the distribution is approximately even with one notable exception
* 7 nop: seven ops have ~800 samples; one has ~10
   * Most even distribution
* 8 nop: one ops has 5720 samples; one has 4 
   * it is even missing most of the loads and only putting all the samples to mainly one ops.


Note: by default the output removes nops lines which as no attribution.
