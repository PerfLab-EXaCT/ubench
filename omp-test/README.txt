-*-Mode: text;-*-

$HeadURL$
$Id$

=============================================================================

OMP_NUM_THREADS=10 \
  ./omp-mm

OMP_NUM_THREADS=10 \
  srun -p ivyall -N 1 --exclusive \
  ./omp-mm

#make-batch-job --app="omp-mm" \
#  --mpi-per-node 1 --omp-per-task 10 \
#  1

=============================================================================

Provenance:

https://computing.llnl.gov/tutorials/openMP/exercise.html

omp-hello.c			Hello world
omp-workshare1.c		Loop work-sharing
omp-workshare2.c		Sections work-sharing
omp-reduction.c			Combined parallel loop reduction
omp-orphan.c			Orphaned parallel loop reduction
omp-mm.c			Matrix multiply
omp-getEnvInfo.c		Get and print environment information

=============================================================================
