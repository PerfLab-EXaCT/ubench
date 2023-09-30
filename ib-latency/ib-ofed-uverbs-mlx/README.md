-*-Mode: markdown;-*-
-----------------------------------------------------------------------------

Benchmarks based on Mellanox Perftest Package
=============================================================================

Mellanox Perftest Package: A set of microbenchmarks for
InfiniBand/RDMA. The Perftest Package is a collection of tests written
over OFED uverbs intended for use as a performance
micro-benchmark. The tests may be used for tuning as well as for
functional testing.

Refs:
  https://community.mellanox.com/s/article/perftest-package
  https://community.mellanox.com/s/article/howto-enable-perftest-package-for-upstream-kernel


Tests
----------------------------------------

* Bandwidth tests: send, write, read

* Latency tests:   send, write, read

* Atomics bandwidth, latency: compare-and-swap, fetch-and-add


Methodology
----------------------------------------

- Each benchmark is run on a pair of nodes, one a server and the other
  a client.

- The benchmarks use the CPU cycle counter to get time stamps without
  context switch.

- The latency benchmarks measure round-trip time but report half of
  that as one-way latency. This means that the results may not be
  accurate for asymmetrical configurations.

- The bandwidth benchmarks test unidirectional bandwidth, where the
  client measures the bandwidth. They test several message sizes and
  measure both bandwidth and message rate.

Also see: https://github.com/linux-rdma/perftest/blob/master/README


Drivers:
=============================================================================

* `run-ib-slurm`: Runs all microbenchmarks on all nodes within a SLURM
  allocation.

* `run-ib-bench-all`: Runs all microbenchmarks in round-robin fashion.

* `run-ib-bench`: Launch a single microbenchmark. Each launch is
  arranged has a server/client pair.


Example
=============================================================================

1. Obtain a SLURM allocation:
   ```
   salloc -N 2 -t 0:60:00 --exclusive

   # 
   ./run-ib-slurm
   ```

Notes/TODO
=============================================================================

Bugs in Perftest microbenchmark
----------------------------------------

1. When using --all and --json output, cannot append to output file!

2. The json format is invalid, i.e., not properly quoted

Workaround:
  Recipe for converting non-quoted json to quoted json:
  https://stackoverflow.com/questions/68682209/parse-json-without-quotes-in-python

  https://stackoverflow.com/questions/21104592/json-to-pandas-dataframe
  https://www.kite.com/python/answers/how-to-convert-a-json-string-to-a-pandas-dataframe-in-python
  https://www.delftstack.com/howto/python-pandas/json-to-pandas-dataframe/


TODO/run-ib-slurm:
----------------------------------------

- Support two modes:
  - Launch on a single node means client/server with head node (current).
  - Launch on two nodes means client/server with those two nodes


Perftest Package, IB Tests, manual invocation
=============================================================================

```
  ib_send_bw 
  ib_write_bw
  ib_read_bw
    
  ib_send_lat
  ib_write_lat
  ib_read_lat

  ib_atomic_bw
  ib_atomic_bw
```


1. Find MLX device: `ibv_devinfo` --> mlx5_0, port 1

2. Find <server_ip>
```
  hostname -i
  ifconfig's inet from eno1 or eno2:
  ip -4 addr show eno1 | \grep -oP '(?<=inet\s)\d+(\.\d+){3}'
```

3. Execute
```
  # Obtain a node:
  salloc -N 1 -t 0:60:00 --exclusive

  # Server node:
  ib_send_bw -d mlx5_0 -i 1 -F -D 5 --report_gbits

  # Client node:
  ib_send_bw -d mlx5_0 -i 1 -F -D 5 --report_gbits <server_ip>
```


Perftest Package, Ethernet Tests, Manual invocation
=============================================================================

This is a collection of tests written using Infiniband verbs intended for use as a performance micro-benchmark. The tests can measure the latency and bandwidth of Infiniband fabrics.

```
raw_ethernet_bw

raw_ethernet_lat
raw_ethernet_burst_lat

raw_ethernet_fs_rate

# raw_ethernet_bw --server -d mlx5_0 -E 28:33:44:55:66:77 -B 77:22:33:44:55:88 --dest_ip 1.1.1.2 --tos <VALUE>

# raw_ethernet_bw --client -d mlx5_0 -B 28:33:44:55:66:77 -E 77:22:33:44:55:88 --dest_ip 1.1.1.2 --tos <VALUE>

-s, --size=<size>  Size of message

```
