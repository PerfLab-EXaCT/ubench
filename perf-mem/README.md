-*-Mode: markdown;-*- [outline]
=============================================================================

=============================================================================
Perf usage:
=============================================================================

Download perf sources from kernel.org and compile only perf. (The new
version supports Intel event names.)

```
perf list
perf mem record -e list

# Perf events (mem_inst_retired.all_loads/mem_uops_retired:all_loads)
# [Use perf report --header to see event translations]
perf record -d -e mem_inst_retired.all_loads:upp -c 9973 -o <out>
perf record -d -e cpu/event=0xd0,umask=0x81/upp -c 9973 -o <out>

# Perf mem events:
perf record    -e cpu/mem-loads/upp 
perf record    -e cpu/mem-loads,ldlat=1/upp
perf record -W -e cpu/mem-loads,ldlat=0/upp -c <cnt> -o <out>
perf record    -e cpu/event=0x0b,umask=0x10/upp 
perf record    -e cpu/config=0x1cd,config1=0x1f,config2=0x0/upp
# -W: sample by weight
# -d: record addresses

perf annotate -n --no-source --full-paths --stdio -i <in>
perf report -n -v -U --no-demangle --stdio -i <in>
perf report --stdio --header -n -i <in>

perf script -F event,dso,ip,sym,symoff -i <input>

perf annotate -n --no-source --full-paths --stdio -i <data>
# duplicates disassembly for each event
```


=============================================================================
SkyLake Data Linear Address Events (perf, Intel)
=============================================================================

Data Linear Address events:
# mem_inst_retired.all_loads       MEM_UOPS_RETIRED.ALL_LOADS
# mem_inst_retired.lock_loads      MEM_UOPS_RETIRED.LOCK_LOADS
# mem_inst_retired.split_loads     MEM_UOPS_RETIRED.SPLIT_LOADS
# mem_inst_retired.stlb_miss_loads MEM_UOPS_RETIRED.STLB_MISS_LOADS

# mem_inst_retired.all_stores       MEM_UOPS_RETIRED.ALL_STORES
# mem_inst_retired.split_stores     MEM_UOPS_RETIRED.SPLIT_STORES
# mem_inst_retired.stlb_miss_stores MEM_UOPS_RETIRED.STLB_MISS_STORES

# mem_load_retired.l1_hit   MEM_LOAD_UOPS_RETIRED.L1_HIT
# mem_load_retired.l2_hit   MEM_LOAD_UOPS_RETIRED.L2_HIT
# mem_load_retired.l3_hit   MEM_LOAD_UOPS_RETIRED.L3_HIT
# mem_load_retired.fb_hit   MEM_LOAD_UOPS_RETIRED.HIT_LFB

# mem_load_retired.l1_miss  MEM_LOAD_UOPS_RETIRED.L1_MISS
# mem_load_retired.l2_miss  MEM_LOAD_UOPS_RETIRED.L2_MISS
# mem_load_retired.l3_miss  MEM_LOAD_UOPS_RETIRED.L3_MISS

# mem_load_l3_hit_retired.xsnp_hit  MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HIT
# mem_load_l3_hit_retired.xsnp_hitm MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HITM
# mem_load_l3_hit_retired.xsnp_miss MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_MISS
# mem_load_l3_hit_retired.xsnp_none MEM_LOAD_UOPS_LLC_HIT_RETIRED.XSNP_NONE

# mem_load_l3_miss_retired.local_dram  MEM_LOAD_UOPS_LLC_MISS_RETIRED.LOCAL_DRAM
# mem_load_l3_miss_retired.remote_dram MEM_LOAD_UOPS_LLC_MISS_RETIRED.REMOTE_DRAM


=============================================================================
Comments:
=============================================================================

* Perf may throttle if sampling frequency is too high.
  Test: `perf report -D -i <input> | grep THROT`


* Perf's multiplexing often gives inaccurate results.


* What have we learned about Intel PMU events?
  - Line replacement events are not precise -> attribution issues

  - The `L2_RQSTS` and `L2_DATA_RQSTS` events can be used to discern assorted access types. In all of the L2 access events the designation PREFETCH only refers to the L2 hardware prefetch. The designation DEMAND includes loads and requests due to the L1D hardware prefetchers. 

  - The event `L1D_PREFETCH.REQUESTS` is counted whenever the DCU attempts to prefetch cache lines from the L2 (or memory) to the DCU. If you expect the DCU prefetchers to work and to count this event, but instead you detect the event `MEM_LOAD_RETIRE.L1D_MISS`, it might be that the IP prefetcher suffers from load instruction address collision of several loads. (739)

Subtract effects of code: `L2_RQSTS.CODE_RD_HIT`


* Prefetch related events do not change when turning the prefetchers on or off!

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
? `offcore_response.all_pf_rfo.any_response`


=============================================================================
Extra
=============================================================================

* New option to specify latency:
  - https://patchwork.kernel.org/patch/9176415/
  > perf mem record -e ldlat-loads --ldlat 50 -v true
  > record -W -d -e cpu/mem-loads,ldlat=50/P true

> kernel.perf_event_paranoid
https://www.tecmint.com/perf-performance-monitoring-and-analysis-tool-for-linux/

likwid
https://github.com/RRZE-HPC/likwid

Intel manual, Section B.4.3.2, page B-23

> perf stat -e r003c ./x
> perf stat -e cpu/event=0x0b,umask=0x10/p ./x
> perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x


=============================================================================
New
=============================================================================

# dtlb_load_misses.miss_causes_a_walk [Load misses in all DTLB levels that cause page walks]
# dtlb_load_misses.stlb_hit [Loads that miss the DTLB and hit the STLB]
# dtlb_load_misses.walk_completed_4k [Demand load Miss in all translation lookaside buffer (TLB) levels causes a page walk that completes (4K)]


???
# PAGE_WALKER_LOADS.DTLB_L1
# PAGE_WALKER_LOADS.DTLB_L2
# PAGE_WALKER_LOADS.DTLB_L3
# PAGE_WALKER_LOADS.DTLB_M

