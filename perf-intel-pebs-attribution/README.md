

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
* 2 nop: six ops have ~1400 samples; two have ~100
* 3 nop: 

* 6 nops: the distribution is approximately even with one notable exception
* 7 nops: distribution is more uneven than all others


Note: by default the output removes nops lines.
