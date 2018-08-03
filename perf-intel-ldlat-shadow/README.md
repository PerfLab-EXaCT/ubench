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
samples are no longer independent. That is, each load is no longer an
equally probable sampling option.

The problem is that the effect of each shadow can be large and
unpredictable. To see this, note that load latencies are dependent on
input, data locality, and memory hierarchy. Further, latencies have
large variability, ranging from a few to hundreds of cycles. Finally,
the number of load instructions in a shadow depends not only on shadow
size, but also on the dynamic instruction stream.

To make a statistically meaningful conclusions from the sampling
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
iterations.  When we compile with GCC 7.1.0 and [[-O0]], we obtain 7 and 8
loads per iteration, resulting in the following expectations:
- loop in f1: 7 load instructions * 10M iterations = 70M loads total
- loop in f2: 8 load instructions * 10M iterations = 80M loads total

[[expect optimization to have more of an effect as it remove frame loads]]


The total loads estimated from DLA samples conforms to these expectations.

The total loads estimated from load latency samples is wildly off.

  [[Should sampling period have an effect?]]
  
  
  How many different types of shadows?

 *** We vary the period of the load latency sampling... to show it has an effect..

(We confirm that perf does not throttle due to overly frequent sampling.)


Terrible: x benchmark with random access

Best case: benchmark y:
 - totals are off; ratio depends on application
 - the distributions are good
Explanation: Frequent L1 hits and a regular instruction stream mean that there is a relatively consistent number of loads in each sample's shadow. The main issue is that the effect cannot be estimated.


  load count       w: 74x          x: 80x           y: 95x
  precise (13973): 1,720,453,571   7,593,920,283    1,897,742,995
  ldlat    (2313):    23,231,772      79,888,707	   17,289,675
  ldlat    (4986):    23,229,774      79,776,000	   17,291,448
  ldlat    (9973):    23,237,090      79,724,162	   17,293,182
  ldlat   (19946):    23,177,252      79,504,756	   17,273,236
  ldlat   (39891):    23,216,562      79,662,327	   17,272,803

       

For w: 
- the distribution of ldlat is uneven

Question: how representative are the latencies?


g++ -std=c++11 -g -O3 -Wall -o w w.cpp


# Event count (approx.): 1,720,453,571
# Event count (approx.):    23,231,772
# Event count (approx.):    23,229,774
# Event count (approx.):    23,237,090
# Event count (approx.):    23,177,252
# Event count (approx.):    23,216,562

# Event count (approx.): 7,593,920,283
# Event count (approx.):    79,888,707
# Event count (approx.):    79,776,000
# Event count (approx.):    79,724,162
# Event count (approx.):    79,504,756
# Event count (approx.):    79,662,327

# Event count (approx.): 1,897,742,995
# Event count (approx.):    17,289,675
# Event count (approx.):    17,291,448
# Event count (approx.):    17,293,182
# Event count (approx.):    17,273,236
# Event count (approx.):    17,272,803
