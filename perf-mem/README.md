-*-Mode: markdown;-*- [outline]
=============================================================================

Samples should be proportional to loads (and independent of cache state).

Problem: only monitor loads over certain latency!

SeaPearl:
--------------------------------------------------
- Intel(R) Xeon(R) CPU E5-2650 v2 @ 2.60GHz
- perf version 2.6.32-504.8.1.el6.x86_64.debug
- event: cpu/mem-loads/pp

        samples            events
period  total   f1   f2    f1    f2
  4986:  253   120  130   600k  650k
  9973:  126    60   65     "     "
 19946:   63    30   32     "     "
 39891:   31    15   16     "     "


BlueSky:
--------------------------------------------------
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



[[New option to specify latency:]]
  - https://patchwork.kernel.org/patch/9176415/
  > perf mem record -e ldlat-loads --ldlat 50 -v true
  > record -W -d -e cpu/mem-loads,ldlat=50/P true

> kernel.perf_event_paranoid
https://www.tecmint.com/perf-performance-monitoring-and-analysis-tool-for-linux/

likwid
https://github.com/RRZE-HPC/likwid


# perf stat -e cycles -e cpu/event=0x0e,umask=0x01,inv,cmask=0x01/ -a sleep 5



cpu/mem-loads/P
