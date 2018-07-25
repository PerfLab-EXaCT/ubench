
Setup:
------------------------------------------------------------

This microbenchmark tests the attribution of Intel's precise PMU event
`mem_inst_retired.all_loads`. For precise events, we expect
attribution to the instruction (dynamically) following the instruction
causing the interrupt. This rule makes it easy for post-mortem
analysis to identify the exact instruction causing the interrupt. (See
"Reduced Skid PEBS" and "Precise Distribution of Instructions Retired"
in Intel Soft Dev Manual; and "Correlating Performance Bottleneck to
Source Location" in Intel Arch Opt manual.)

Here is the issue: we have found that using precise events (a) we can
accurately estimate the total number of events (e.g., loads), but that
(b) the attribution does _not_ conform to expectations. For instance
on a loop with a straight line sequence of loads, one would expect a
uniform distribution of samples. We obtain highly non-uniform
distributions.

One hypothesis was that when multiple load ups retired in the same
cycle, PEBS would attribute to the first load instead of the load
actually causing the interrupt. (We assume for simplicity a dynamic
corrective action that identifies the exact instruction.)

To test the attribution, we used the simple code in `y.c`. Functions
f1 and f2 each contain one loop with a straight-line sequence of
loads. We compiled the C code into assembly using GCC 7.1.0 with
-O0. We then inserted from 0 to 8 nops just after each instruction
containing a load uop; the code is in `y-Xnop.s`.

We profiled the resulting binaries using "perf record":
  > 1: perf record -e mem_inst_retired.all_loads:upp -c <period>
  > 2: perf record -e cpu/event=0xd0,umask=0x81/upp -c <period>

Our sample results are in the directory `intel-xeon-6126`. The `dla1`
and `dla2` versions correspond to the perf invocations 1 and 2,
respectively.


Results for load totals:
------------------------------------------------------------

We expect:
- the loop in f1 to have 70M loads (10M iterations; 7 load instructions).
- the loop in f2 to have 80M loads (10M iterations; 8 load instructions).

The total samples conforms to these expectations.


Results for load attribution:
------------------------------------------------------------

We expect a uniform distribution of samples. In particular, we expect
each load to have 715 samples. (Each loop has 10M iterations; sample
period is 13973.)

We cannot explain the distribution of loads.

For example, consider the following summary of the attribution to
function f2 using approximate histograms:

- 0 nop: 1800, 1500, 800 x 2, 600, 150, 10, 0 (attribution to jump!)
- 1 nop: 800 x 6, 400 x 2
- 2 nop: 1400 x 4, 100 x 2, 50, 5
- 3 nop: 1250; 900 x 2, 600 x 3, 540; 420
- 4 nop: 1450 x 2, 1300 x 2, 120; 20, 10, 1
- 5 nop: 1000 x 2, 900 x 4, 50, 30
- 6 nop: 800 x 7, 25
- 7 nop: 5200, 350, 180, 15 x 2, 3, 1, 0
- 8 nop: 3800, 1850, 20 x 3, 5, 3, 0

(Note: the output removes nops lines which as no attribution.)
