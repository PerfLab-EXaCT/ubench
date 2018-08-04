-----------------------------------------------------------------------------
Summary:
-----------------------------------------------------------------------------

This microbenchmark tests sampling using Intel's load latency
event. The load latency event gathers extremely interesting
information about loads. However, there are questions about making
statistically rigorous conclusions using its data.

The load latency mechanism tracks a load until it retires. While
tracking, it ignores other loads.  We say the ignored loads fall into
the load latency _shadow_. We believe the shadow creates a sampling
bias because samples are no longer independent. That is, each load is
no longer an equally probable sampling option. The sampling bias does
not seem to be as bad as we expect, but we do not know why.

The basic problem, as we understand it, is that the effect of each
shadow can be large and unpredictable. To see this, note that load
latencies are dependent on input, data locality, and memory
hierarchy. Further, latencies have large variability, ranging from a
few to hundreds of cycles. Finally, the number of load instructions in
a shadow depends not only on shadow size, but also on the dynamic
instruction stream.

To make a statistically meaningful conclusions from the sampling
population we must be able to argue that the sampled population
represents the actual population. As there is no way to estimate the
bias of each sample's shadow, there is no way to sensibly correlate
the samples with the actual population. In a code with many
long-latency loads, the sampling may shadow so many loads that the
data is meaningless.

We use microbenchmarks to test for two things. 
- First, can we obtain an accurate total load count? We compare data
  gathered from the load latency event and the DLA event
  `mem_inst_retired.all_loads`.
- Second, we consider the distribution of samples on straight line
  code. We expect a uniform distribution. (Note that a separate study
  has shown that `mem_inst_retired.all_loads` does _not_ yield uniform
  distributions!)
A third interest is validating the memory latency data, but we ignore
that for now.

We tested using three microbenchmarks.
- w.cpp: indirect loads (via pointers), many conditionals, vary
  compute-heavy (f2) and conditional-load heavy (f1)
- x.c: indirect loads (via arrays) and conditionals
- y.c: indirect loads (via arrays) with straight-line code

We compile with GCC 7.1.0; see the Makefile. We compile w.cpp with
optimization to remove frame loads in f2.

We profiled the resulting binaries using "perf record":
  > 1: perf record -e mem_inst_retired.all_loads:upp -c <period>
  > 2: perf record -W -e cpu/mem-loads,ldlat=0/upp -c <period>

System:
- SkyLake (Xeon Gold 6126, 2.60GHz)
- CentOS Linux release 7.4.1708 (kernel 3.10.0-693.5.2.el7.x86_64)
- perf 4.14.38

Our sample load counts are in the directory `intel-xeon-6126`. Here is
a summary, showing the different events/periods for each
microbenchmark. We confirm that perf does not throttle due to overly
frequent sampling.

|                  microbenchmark and ratio of dla to ldlat totals
| event (period)   w: 66x           x: 96x           y: 111x
|----------------------------------------------------------------
| precise (13973): 2,331,017,779    7,593,920,283    1,897,742,995
| ldlat    (2313):    35,277,876       79,888,707	   	17,289,675
| ldlat    (4986):    35,280,936       79,776,000	   	17,291,448
| ldlat    (9973):    35,284,474       79,724,162	   	17,293,182
| ldlat   (19946):    35,244,582       79,504,756	   	17,273,236
| ldlat   (39891):    35,303,535       79,662,327	   	17,272,803


-----------------------------------------------------------------------------
Observations:
-----------------------------------------------------------------------------

1. The total loads estimated from load latency samples is wildly below
   those estimated from DLA samples. The ratios (shown above) are
   application dependent and range from a _factor_ of 66
   to 111. (Separately, we have validated DLA samples against
   microbenchmarks.) We expect that these ratios correspond to the
   number of loads shadowed while tracking a sampled load. The ratios
   show that there is a gap between load latency samples and the true
   population.

2. We varied the sampling period of of the load latency event,
   expecting that it would affect the totals. It turns out that the
   sampling period does _not_ significantly affect the estimated
   samples. This result suggests that the stream of executed loads,
   even in benchmark w, is uniform enough that the sampling period
   does not affect the bias of the shadow. That is good news.

3. The distribution of the load latency samples in straight-line code
   (y) is quite even. This result surprised us, though it is good
   news.

4. We did not validate the latency histogram obtained by sampling, or
   whether it is affected by period. However, observation 3 (even
   distribution of samples) suggests that, at least for straight-line
   code, the samples should produce a representative latency
   histogram.
  

