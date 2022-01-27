-*-Mode: markdown;-*-
-----------------------------------------------------------------------------

Perf
=============================================================================

Download kernel sources (Symbolic Intel events in versions 4.8+)

  ```sh
  cd <linux-4.xx>/tools/perf
  make help
  make prefix=<prefix> install
  # make prefix=${HOME}/pkg/perf-4.16.7/${MYCFG_SYSNAMESHORT} install
  # make prefix=${HOME}/pkg/perf-4.14.38/${MYCFG_SYSNAMESHORT} install
  ```

Perf usage
=============================================================================

```
perf list # show events

perf record 
  -a: all cpus

perf report --header # show event translations

perf report -n -v -U --no-demangle --stdio -i <in>
perf report --stdio --header -n -i <in>

perf annotate -n --no-source --full-paths --stdio -i <data>
# duplicates disassembly for each event
```


Intel (PEBS) Data Linear Address (DLA) events
=============================================================================

```sh
perf record -d -e mem_inst_retired.all_loads:upp -c 9973 -o <out>
perf record -d -e cpu/event=0xd0,umask=0x81/upp -c 9973 -o <out>

perf script -F event,ip,sym,symoff,dso -i <input>
```

Data Linear Address events:
```
  mem_inst_retired.all_loads   MEM_UOPS_RETIRED.ALL_LOADS
  mem_inst_retired.all_stores  MEM_UOPS_RETIRED.ALL_STORES

  mem_load_retired.l1_hit   MEM_LOAD_UOPS_RETIRED.L1_HIT
  mem_load_retired.l2_hit   MEM_LOAD_UOPS_RETIRED.L2_HIT
  mem_load_retired.l3_hit   MEM_LOAD_UOPS_RETIRED.L3_HIT
  mem_load_retired.fb_hit   MEM_LOAD_UOPS_RETIRED.HIT_LFB

  mem_load_retired.l1_miss  MEM_LOAD_UOPS_RETIRED.L1_MISS
  mem_load_retired.l2_miss  MEM_LOAD_UOPS_RETIRED.L2_MISS
  mem_load_retired.l3_miss  MEM_LOAD_UOPS_RETIRED.L3_MISS

  mem_inst_retired.lock_loads      MEM_UOPS_RETIRED.LOCK_LOADS
  mem_load_l3_hit_retired.xsnp_hit  MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HIT
  mem_load_l3_hit_retired.xsnp_hitm MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_HITM
  mem_load_l3_hit_retired.xsnp_miss MEM_LOAD_UOPS_L3_HIT_RETIRED.XSNP_MISS
  mem_load_l3_hit_retired.xsnp_none MEM_LOAD_UOPS_LLC_HIT_RETIRED.XSNP_NONE

  mem_load_l3_miss_retired.local_dram
  mem_load_l3_miss_retired.remote_dram

  mem_inst_retired.stlb_miss_loads: Retired load insns that miss the STLB (addr)
  mem_inst_retired.stlb_miss_stores

  mem_inst_retired.split_loads     MEM_UOPS_RETIRED.SPLIT_LOADS
  mem_inst_retired.split_stores    MEM_UOPS_RETIRED.SPLIT_STORES
```


Intel (PEBS) Load Latency events
=============================================================================

```sh
See: /sys/bus/event_source/devices/cpu/events/mem-{loads,stores}

perf mem uses /sys/devices/cpu/events/mem-{loads,stores}

perf record       -e cpu/mem-loads/upp
perf record       -e cpu/mem-loads,ldlat=1/upp
perf record -d -W -e cpu/mem-loads,ldlat=1,period=<cnt>/upp
perf record -d -W -e cpu/mem-loads,ldlat=1/upp -c <cnt>
perf record       -e cpu/event=0x0b,umask=0x10/upp
perf record       -e cpu/config=0x1cd,config1=0x1f,config2=0x0/upp
# -W: sample by weight
# -d: record addresses

# Equivalent
perf mem record       -e ldlat-loads --ldlat 50
perf     record -d -W -e cpu/mem-loads,ldlat=50/P

perf script -F event,addr,bpf-output,ip,sym,symoff,dso -i <input>
```


NEW: last branch record


AMD IBS/LWP
=============================================================================

Reports physical/virtual memory addresses for memory insn
  IBS Data Cache Linear Address Register (IbsDcLinAd)
  IBS Data Cache Physical Address Register (IbsDcPhysAd)
  Last Branch Record: one slot

Linux kernel "Dazed and confused"
  https://www.spinics.net/lists/kernel/msg3870271.html


AMD Toolkit:
  https://github.com/jlgreathouse/AMD_IBS_Toolkit/
  https://github.com/jlgreathouse/AMD_IBS_Toolkit/blob/master/README.txt
  
  * Does not support PIE executables, which are standard on Ubuntu. Compile with `-no-pie`.
  
  <joseph.l.greathouse@gmail.com>
  
  ```sh
  sudo <amd-tk>/driver/install_ibs_driver.sh
  
  Collect IBS data and source annotation:
    # Generic:
    <amd-tk>/tools/ibs_run_and_annotate/ibs_run_and_annotate -o -f -- <app>
    
    # Example: run and collect
    <amd-tk>/tools/ibs_run_and_annotate/ibs_run_and_annotate -o 10400 -- <app>
    
    # Example: source annotate only
    <amd-tk>/tools/ibs_run_and_annotate/ibs_run_and_annotate --lookup -o 10400 -- <app>

  Directly collect IBS data and convert to CSV:
    <amd-tk>/tools/ibs_monitor/ibs_monitor -o app.op -f app.fetch <app>
    <amd-tk>/tools/ibs_decoder/ibs_decoder -i app.op -o op.csv -f app.fetch -g fetch.csv
    
  # Final winnowing
    <palm>/amd-ibs-select <in.csv> -o <out.csv>
  ```

Linux perf:
```
  See: /sys/bus/event_source/devices/{ibs_fetch,ibs_op}
  
  'perf mem' fails; needs /sys/devices/cpu/events/mem-{loads,stores}

  perf record [-R] -e ibs_op//p    -a <app>
  perf record [-R] -e ibs_fetch//p -a <app>

    period: -c <cnt>,  period=<cnt>
    ???  rand_en=1
    ???  cnt_ctl=1
    
    system-wide: -a
    per process: -p $(pidof <app>) --> does not seem to work


  ---
  perf record [-R] -e ibs_op//p -p $(pidof <app>)
  
  perf record -vv -R -e ibs_op//p -a -C 0 taskset -c 0 true
  perf record     -R -e ibs_op//p -a sleep 1

  perf report -D --stdio --header
```


Comments:
=============================================================================

* Perf may throttle if sampling frequency is too high.
  Test: `perf report -D -i <input> | grep THROTTLE`


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



Extra
=============================================================================

* New option to specify latency:
  - https://patchwork.kernel.org/patch/9176415/
  > perf mem record -e ldlat-loads --ldlat 50     <app>
  > perf record -W -d -e cpu/mem-loads,ldlat=50/P <app>

> kernel.perf_event_paranoid
https://www.tecmint.com/perf-performance-monitoring-and-analysis-tool-for-linux/

likwid
https://github.com/RRZE-HPC/likwid

Intel manual, Section B.4.3.2, page B-23

> perf stat -e r003c ./x
> perf stat -e cpu/event=0x0b,umask=0x10/p ./x
> perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x



New
=============================================================================

A page table walk occurs when a TLB lookup fails.

SkyLake: dTLB has 64 entries (4 KiB); iTLB has 128; TLB/2 (unified) has 1536.

A dTLB/1 lookup fails after 64 pages = 256 KB (less than L2).
A dTLB/2 lookup fails after 1536 pages = 6.1 MB (assuming inclusive; less than L3).


The page tables in the Intel 64 architecture make very good use of caches, with each cache line holding Page Table Entries (PTEs) for 8 consecutive (virtual address) pages.  So it is quite common for all page table walks to find the data in the caches, rather than going all the way to memory.
```
mem_inst_retired.stlb_miss_loads: Retired load insns that miss the STLB (addr)
```

This is similar to using load-lat with a high threshold.

Problem: cannot get every memory load. We *can* get a representative sample of memory loads. That is we expect both stlb and high-latencies to be uniformly distributed across memory loads. But, we cannot infer from that sample when there is reuse.


Intel manual, SkyLake (Table 19-4; page 19-25)
```
  dtlb_load_misses.miss_causes_a_walk Load misses in all DTLB levels that cause page walks]
  dtlb_load_misses.walk_completed_4k: Load misses in all TLB levels causing a page walk that completes (4K)

  dtlb_load_misses.stlb_hit: Loads that miss the DTLB and hit the STLB
```

Intel manual, Broadwell (Table 19-7; page 19-43)
```
PAGE_WALKER_LOADS.DTLB_L1: DTLB page walker loads that hit in the L1+FB
PAGE_WALKER_LOADS.DTLB_L2
PAGE_WALKER_LOADS.DTLB_L3
PAGE_WALKER_LOADS.DTLB_MEMORY: Number of DTLB page walker loads from memory
```
