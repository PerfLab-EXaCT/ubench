-*-Mode: markdown;-*- [outline]
=============================================================================

Vince Weaver's code provides third-party control of prefetchers:
  https://github.com/deater/uarch-configure/tree/master/intel-prefetch

  ./intel-prefetch-ctl -d
  ./intel-prefetch-ctl -e


For first-party control of prefetchers, need to use sched_getaffinity()
  cf. https://stackoverflow.com/questions/4586405/how-to-get-the-number-of-cpus-in-linux-using-c?answertab=active#tab-top


=============================================================================
Intel model numbers:
=============================================================================

https://en.wikichip.org/wiki/intel/cpuid

https://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbers

=============================================================================
Controlling Intel prefetchers:
=============================================================================

https://01.org/msr-tools/downloads/msr-tools-source-code
  
/dev/cpu/<core#>/msr


Details for Nehalem and post-Nehalem:
----------------------------------------

https://software.intel.com/en-us/articles/disclosure-of-hw-prefetcher-control-on-some-intel-processors

There is a Model Specific Register (MSR) on every core with address of 0x1A4 that can be used to control these 4 prefetchers. Bits 0-3 in this register can be used to either enable or disable these prefetchers. Other bits of this MSR are reserved.

If any of the above bits are set to 1 on a core, then that particular prefetcher on that core is disabled. Clearing that bit (setting it to 0) will enable the corresponding prefetcher. Please note that this MSR is present in every core and changes made to the MSR of a core will impact the prefetchers only in that core. If hyper-threading is enabled, both the threads share the same MSR.


Example for pre-Nehalem:
----------------------------------------

The following enables the hardware prefetcher (by unsetting bit 9):
> ./wrmsr -p 0 0x1a0 0x60628e2089
> ./rdmsr      0x1a0 0x60628e2089

The following disables the hardware prefetcher (by enabling bit 9):
> ./wrmsr -p 0 0x1a0 0x60628e2289
> ./rdmsr      0x1a0 0x60628e2289


