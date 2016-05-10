-*-Mode: text;-*-

$HeadURL$
$Id$

=============================================================================

make-batch-job --app="mpi-costs" \
  --time 5 \
  --mpi-per-node 1 \
  4

srun -p ivy -N 5 --exclusive mpiexec -n 100 ./mpi-costs

=============================================================================

-----------------------------------------------------------------------------
timer costs
-----------------------------------------------------------------------------

10,000,000 iterations

monotonic:
  GA   : get-thread-data,    time/call (us): 0.006026
  GA   : get-metric-id-fast, time/call (us): 0.000000
  GA   : get-metric-id-slow, time/call (us): 0.004067
  GA   : get-time-real,      time/call (us): 0.190827
  GA   : get-time-monotonic, time/call (us): 0.192518

  GA   : get-thread-data,    time/call (us): 0.006066
  GA   : get-metric-id-fast, time/call (us): 0.000000
  GA   : get-metric-id-slow, time/call (us): 0.003082
  GA   : get-time-real,      time/call (us): 0.186959
  GA   : get-time-monotonic, time/call (us): 0.193303

realtime:
  GA   : get-thread-data,    time/call (us): 0.006302 [using clock_getTime()]
  GA   : get-metric-id-fast, time/call (us): 0.000000 "
  GA   : get-metric-id-slow, time/call (us): 0.004304 "
  GA   : get-time-real,      time/call (us): 0.193339 "
  GA   : get-time-monotonic, time/call (us): 0.194191 "
  GA   : getTSC,             time/call (us): 0.022341 [using clock_getTime()]
  GA   : getTSC,             time/call (us): 0.014508 [using getTSC]

  GA   : get-thread-data,    time/call (us): 0.006069 [using clock_getTime()]
  GA   : get-metric-id-fast, time/call (us): 0.000000 "
  GA   : get-metric-id-slow, time/call (us): 0.003141 "
  GA   : get-time-real,      time/call (us): 0.190722 "
  GA   : get-time-monotonic, time/call (us): 0.194667 "
  GA   : getTSC,             time/call (us): 0.022335 [using clock_getTime()]
  GA   : getTSC,             time/call (us): 0.014396 [using getTSC]

  GA   : get-time-real,      time/call (cyc): 417.482464
  GA   : get-time-real,      time/call (cyc): 419.220561
  GA   : getTSC,             time/call (cyc): 30.721989
  GA   : getTSC,             time/call (cyc): 30.245935
