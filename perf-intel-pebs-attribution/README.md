-----------------------------------------------------------------------------
Summary:
-----------------------------------------------------------------------------

This microbenchmark tests the attribution of Intel's precise PMU event
`mem_inst_retired.all_loads`. For precise events, we expect
attribution to the instruction (dynamically) following the instruction
causing the interrupt. This rule makes it easy for post-mortem
analysis to identify the exact instruction causing the interrupt. See
"Reduced Skid PEBS" and "Precise Distribution of Instructions Retired"
in Intel Soft Dev Manual; and "Correlating Performance Bottleneck to
Source Location" in Intel Arch Opt manual.

Here is the issue: we have found that using precise events (a) we can
accurately estimate the total number of events (e.g., loads), but that
(b) the attribution does _not_ conform to expectations. For instance
on a loop with a straight line sequence of loads, one would expect a
uniform distribution of samples. We obtain highly non-uniform
distributions.

One hypothesis is that retiring multiple instructions per cycle
affects attribution. For example, what happens when multiple load ups
retire in the same group? If the second load triggers the interrupt,
does PEBS attribute to the first? (For simplicity, we are assuming
PEBS next-instruction attribution is auto-corrected).

To test the attribution, we used the simple code in `x.c`. Functions
f1 and f2 each contain one loop with a straight-line sequence of
loads. To avoid as much out-of-order retirement as possible -- could
this be affecting attribution? -- we wanted predictable memory
latencies and loop carried dependences. Function f1 accesses an array
in a simple stride-1 pattern for predictable memory latency. Function
f2 additionally uses reduction so that loop iterations are sequential.

To obtain retirement bundles where only one load completes, we created
variants of this microbenchmark with 0 to 8 nops inserted just after
each instruction containing a load uop. The code is in `x-Xnop.s`.

We compiled the C and assembly code using GCC 7.1.0 with -O0.

We profiled the resulting binaries using "perf record":
  > 1: perf record -e mem_inst_retired.all_loads:upp -c <period>
  > 2: perf record -e cpu/event=0xd0,umask=0x81/upp -c <period>

Our sample results are in the directory `intel-xeon-6126`. The `dla1`
and `dla2` versions correspond to the perf invocations 1 and 2,
respectively.

Summary: We cannot explain the resulting distributions if PEBS
guarantees a fixed skid of 1. The nops should retire in order, even if
they can execute in parallel with every load. We expect that PEBS
actually has a variable skid, even if the skid is smaller than
non-precise events.


-----------------------------------------------------------------------------
Results for 'x' tests
-----------------------------------------------------------------------------

------------------------------------------------------------
Results for load totals:
------------------------------------------------------------

We expect:
- the loop in f1 to have 70M loads (10M iterations; 7 load instructions).
- the loop in f2 to have 70M loads (10M iterations; 7 load instructions).

The total samples conforms to these expectations.


------------------------------------------------------------
Results for load attribution, Function f1:
------------------------------------------------------------

We expect a uniform distribution of samples. Nominally, we expect each
load to have 715 samples. (Each loop has 10M iterations; sample period
is 13973.)

We cannot explain the distribution of loads.

For example, consider the following summary of the attribution to
function f1 using approximate histograms:

- 0 nop: 200x3 414   568    757   2597 (attribution to jump!)
- 1 nop: 234   500x3 700x2  1706
- 2 nop: 500x2 700x3 858    1026
- 3 nop: 225   700x4 928    1084
- 4 nop: 100x2 842   1000x4
- 5 nop: 2     50x2  1100x2 1300x2
- 6 nop: 44    700x2 900x4
- 7 nop: 200x2 727   915    1361  1581
- 8 nop: 300x2 517   900x3  1167


------------------------------------------------------------
Results for load attribution, Function f2:
------------------------------------------------------------

We expect a uniform distribution of samples. Again, we expect each
load to have 715 samples. (Each loop has 10M iterations; sample period
is 13973.)

We cannot explain the distribution of loads.

For example, consider the following summary of the attribution to
function f2 using approximate histograms:

- 0 nop: 2     422   639   1021  2919 (attribution to jump!)
- 1 nop: 132   500x4 815   1921
- 2 nop: 394   500x2 700x2 1100x2
- 3 nop: 100x2 900x4 1015
- 4 nop: 3     150x4 2100x2
- 5 nop: 10x3  71    189   1670  3056
- 6 nop: 5     100x5 4437
- 7 nop: 50x2  120   1200x4
- 8 nop: 50x3  1200x4

(Note: the output removes nops lines which as no attribution.)




-----------------------------------------------------------------------------
Extra: Results for 'y' tests
-----------------------------------------------------------------------------

------------------------------------------------------------
Results for load totals:
------------------------------------------------------------

We expect:
- the loop in f1 to have 70M loads (10M iterations; 7 load instructions).
- the loop in f2 to have 80M loads (10M iterations; 8 load instructions).

The total samples conforms to these expectations.


------------------------------------------------------------
Results for load attribution, Function f1:
------------------------------------------------------------

We expect a uniform distribution of samples. Nominally, we expect each
load to have 715 samples. (Each loop has 10M iterations; sample period
is 13973.)


------------------------------------------------------------
Results for load attribution, Function f2:
------------------------------------------------------------

We expect a uniform distribution of samples. Again, we expect each
load to have 715 samples. (Each loop has 10M iterations; sample period
is 13973.)

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

