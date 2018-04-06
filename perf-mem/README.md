-*-Mode: markdown;-*- [outline]
=============================================================================

Samples should be proportional to loads (and independent of cache state).

Problem: only monitor loads over certain latency!

-----------------------------------------------------------------------------
SeaPearl:
-----------------------------------------------------------------------------
- Intel(R) Xeon(R) CPU E5-2650 v2 @ 2.60GHz
- perf version 2.6.32-504.8.1.el6.x86_64.debug
- event: cpu/mem-loads/pp

        samples            events
period  total   f1   f2    f1    f2
  4986:  253   120  130   600k  650k
  9973:  126    60   65     "     "
 19946:   63    30   32     "     "
 39891:   31    15   16     "     "


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
- event: cpu/mem-loads,ldlat=30/P

        samples            events
period  total   f1   f2    f1    f2
  4986:  152    96   51    .     .
  9973:
 19946:
 39891:


[[Perf bug!]]
  - perf record will not accept -o
  - perf record will not accept -c
  - event is hard wired to: cpu/mem-loads,ldlat=30/P



perf record -e cpu/mem-loads,ldlat=1/P -c 1 ./x

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

-----------------------------------------------------------------------------

Intel manual, Section B.4.3.2, page B-23

> perf stat -e cpu/event=0x0b,umask=0x10/p ./x

> perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ -a sleep 5

> perf stat -e r003c ./x
> perf stat -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ ./x


type = 4,
config=0x1cd,config1=0x3
config2=0x0,
excl_usr=0,excl_kern=0,excl_host=0,excl_guest=1,
precise_ip=2,
attr_mmap2=0,attr_mmap=1,attr_mmap_data=1


# event : name = cpu/config=0x1cd,config1=0x1f,config2=0x0/P, , type = 4, size = 112, config = 0x1cd, { sample_period, sample_freq } = 4000, sample_type = IP|TID|TIME|PERIOD, disabled = 1, inherit = 1, mmap = 1, comm = 1, freq = 1, enable_on_exec = 1, task = 1, precise_i


name = cpu/mem-loads,ldlat=30/P,
type = 4,
size = 112,
config = 0x1cd,
{ sample_period, sample_freq } = 4000,
sample_type = IP|TID|TIME|ADDR|PERIOD|DATA_SRC|WEIGHT,
disabled = 1,
inherit = 1,
mmap = 1,
comm = 1,
freq = 1,
enable_on_exec = 1,
task = 1, 
precise_ip = 3,
mmap_data = 1,
sample_id_all = 1,
mmap2 = 1,
comm_exec = 1,
{ bp_addr, config 1} = 0x1f
