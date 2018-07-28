-----------------------------------------------------------------------------
Summary:
-----------------------------------------------------------------------------

This microbenchmark tests sampling using Intel load latency. The load
latency gathers extremely interesting information about loads. However,
we believe no statistically rigorous conclusions can be made with
its data.

The load latency mechanism tracks a load until it retires. While
tracking, it ignores other loads.  We say the ignored loads fall into
the load latency "shadow." The shadow creates a sampling bias because
samples are no longer independent, i.e., equally probable.

To see this note that load latencies are effectively unpredictable
(i.e., dependent on input, data locality, and memory hierarchy). Even
worse, latencies have large variability, ranging from a few to
hundreds of cycles. As a result, the shadow size has unpredictable and
large variability. Finally, the number of load instructions in the
shadow is not only a function of the shadow size. It also depends on
the dynamic instruction stream.

To make statistically meaningful conclusions from the sampling
population we must be able to argue that the sampled population
represents the actual population. As there is no way to estimate the
bias of each sample's shadow, there is no way to sensibly correlate
the samples with the actual population. In a code with many loads
(typical) and any thing other than L1 hits, the sampling will shadow
so many loads that the data is likely meaningless.

One way to see this is to consider the following microbenchmark. Our
only goal is to obtain an accurate total load count. We compare data
gathered from the load latency event and the DLA event
`mem_inst_retired.all_loads`.

The code is in `y.c`. Functions f1 and f2 each contain one loop with a
straight-line sequence of loads. Each loop has iterates 10M
iterations.  When we compile with GCC 7.1.0 and -O0, we obtain 7 and 8
loads per iteration, resulting in the following expectations:
- loop in f1: 7 load instructions * 10M iterations = 70M loads total
- loop in f2: 8 load instructions * 10M iterations = 80M loads total


The total loads estimated the DLA samples conforms to these expectations.

The total loads estimated from load latency samples is wildly off.

 *** We vary the period of the load latency sampling... to show it has an effect..

(We confirm that perf does not throttle due to overly frequent sampling.)

