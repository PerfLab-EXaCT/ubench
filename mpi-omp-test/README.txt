-*-Mode: text;-*-

$HeadURL: https://svn.pnl.gov/svn/hpcgroup/projects/modsim-tools/ubench/mpi-costs/README.txt $
$Id$

=============================================================================

make-batch-job --app="mpi-omp-mm" \
  --mpi-per-node 2 --omp-per-task 2 \
  --time 10 \
  2


OMP_NUM_THREADS="4" OMP_PROC_BIND="true" \
  srun -p ivy -N 2 --exclusive mpiexec -ppn 1 -n 2 ./mpi-omp-mm

=============================================================================
