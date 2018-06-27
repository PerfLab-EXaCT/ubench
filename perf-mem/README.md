-*-Mode: markdown;-*- [outline]
=============================================================================

What have we learned about perf? multi-plexing gives inaccurate results

What have we learned about events?
  - line replacement events are not precise -> attribution issues

* The L2_RQSTS and L2_DATA_RQSTS events can be used to discern assorted access types. In all of the L2 access events the designation PREFETCH only refers to the L2 hardware prefetch. The designation DEMAND includes loads and requests due to the L1D hardware prefetchers. 

* The event L1D_PREFETCH.REQUESTS is counted whenever the DCU attempts to prefetch cache lines from the L2 (or memory) to the DCU. If you expect the DCU prefetchers to work and to count this event, but instead you detect the event MEM_LOAD_RETIRE.L1D_MISS, it might be that the IP prefetcher suffers from load instruction address collision of several loads. (739)

Substract effects of code: L2_RQSTS.CODE_RD_HIT


- How to parse data? Perhaps use hpctoolkit, since we are focused on profiles.
  -	perf annotate -n --no-source --full-paths --stdio -i <data>
    - duplicates disassembly for each event

  - perf script -F dso,event,ip -i <data>
    - doesn't normalize dso offsets
    - duplicates dso name

- validation:
  - rule: lines-replaced <= loads <= words-in-lines-replaced
  - rule: 


* static analysis: tag base-pointer based loads as touch stack with a working set size of 1


=============================================================================
Skylake prefetchers:
=============================================================================

same when prefetchers on/off:
  `l1d.replacement`

less when prefetchers off:
  `offcore_response.pf_l1d_and_sw.any_response`

zero when prefetchers off:
  `l2_rqsts.all_pf`: seems to double count some prefetch events (e.g. L1 attempt, which turns into and L2
  
  `offcore_response.all_pf_data_rd.any_response`: does not not appear to count all prefethes... only L2?


                         on     off
`l1d.replacement`        60/57   59/56 = 598K * 64 B/line = 38 MB
`l2_rqsts.all_pf`        59/2    0
`offcore_response.pf_l1d_and_sw.any_response`  0  0
`offcore_response.all_pf_data_rd.any_response` 0  0


? `l2_lines_in.all`

/files0/kili337/Experiments/tests/xr.c


? `offcore_response.all_pf_rfo.any_response`


=============================================================================
DLA events:
=============================================================================

Samples should be proportional to loads (and independent of cache state).

Problem: only monitor loads over certain latency!

-----------------------------------------------------------------------------
SeaPearl:
-----------------------------------------------------------------------------
- Intel(R) Xeon(R) CPU E5-2650 v2 @ 2.60GHz
- perf version 2.6.32-504.8.1.el6.x86_64.debug


event: cpu/mem-loads/pp
----------------------------------------

              samples            events
period/sz     total    f1    f2    f1    f2
  4986/10M     253    120   130   600k  650k
  9973/10M     126     60    65     "     "
 19946/10M      63     30    32     "     "
 39891/10M      31     15    16     "     "

PIN tool (MIAMI memreuse):
  f1:    loads: 60M   stores: 30M
  f1+f2: loads: 130M  stores: 60M

Two back-to-back runs of "perf stat"
  5143312 cpu/mem-loads,ldlat=1/pp
  1600768 cpu/config=0x1cd,config1=0x3,config2=0x0/pp
  1266199 cpu/mem-loads,ldlat=1/pp
  1264178 cpu/config=0x1cd,config1=0x3,config2=0x0/pp


-----------------------------------------------------------------------------
BlueSky:
-----------------------------------------------------------------------------
- Intel(R) Xeon(R) Gold 6126 CPU @ 2.60GHz
- perf version 3.10.0-514.21.1.el7.x86_64.debug

event: cpu/mem-loads,.../P
----------------------------------------

             samples               events
period/sz     total    f1    f2     f1    f2
  4986/10M     257    116   140    600k  650k
  9973/10M     128     58    70
 19946/10M      64     29    35
 39891/10M      32     14    18

  4986/100M   2574   1178  1396      6m  6.5m

  12,833,964


event: mem_inst_retired.all_loads
----------------------------------------
              samples             events
period/sz     total    f1    f2    f1    f2
  code/10M    13018   6009  7009   60M   70M   (130M)  [[good!]]
 loads/10M    1919    1355   564  13.5M   5.6M (19M)   [[throttling]]

[[Perf bug!]]
  - perf mem record will not accept -o
  - perf mem record will not accept -c
  - event is hard wired to: cpu/mem-loads,ldlat=30/P

perf mem record -e list

-----------------------------------------------------------------------------
Extra:
-----------------------------------------------------------------------------

* New option to specify latency:
  - https://patchwork.kernel.org/patch/9176415/
  > perf mem record -e ldlat-loads --ldlat 50 -v true
  > record -W -d -e cpu/mem-loads,ldlat=50/P true

> kernel.perf_event_paranoid
https://www.tecmint.com/perf-performance-monitoring-and-analysis-tool-for-linux/

likwid
https://github.com/RRZE-HPC/likwid

Intel manual, Section B.4.3.2, page B-23

-----------------------------------------------------------------------------

> perf stat -e r003c ./x
> perf stat -e cpu/event=0x0b,umask=0x10/p ./x
> perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x

