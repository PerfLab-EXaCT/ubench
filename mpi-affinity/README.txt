-*-Mode: markdown;-*- [outline]
=============================================================================

$HeadURL$
Id: README.txt 14144 2016-05-10 20:39:39Z tallent $

=============================================================================

Nathan Tallent

=============================================================================
Test binding of MPI ranks to cores.  Focus on SLURM + [OpenMPI | MVAPICH2].
=============================================================================

> make-batch-job --app="./mpi-affinity" --mpi-per-node 12  12
> make-batch-job --app="./mpi-affinity" --mpi-per-node 24  12

> srun -N 1 -n 12 --exclusive ./mpi-affinity


To use the SLURM script, 'run.sbatch', do the following:
----------------------------------------

0. Prepare environment:
   source results/<machine>.<mpi>.sourceme.sh

1. Build 'mpi-affinity':
   make

2. Check and submit the SLURM script:
   sbatch [-A PAL] [-p ivy] --ntasks-per-node=40 run.sbatch


=============================================================================

SeaPearl conclusions:
-----------------------------------------------------------------------------

0. numa nodes are numbered in an odd way.

1. srun's binding option does *not* work (at all). [!!!]

2. numactl's binding options work only for non-MPI applications.

3. MPI
   - MVAPICH2 supports explicit binding via MV2_CPU_MAPPING.
   - OpenMPI ...

PIC conclusions:
-----------------------------------------------------------------------------

1. srun's binding options work only for non-MPI applications.  Both
   MVAPICH and OpenMPI reset thread affinity.
   - srun's auto-binding options do not recognize the Interlagos
     shared FPU.
   - srun's explicit binding option (--cpu_bind=map_cpu) works for
     non-MPI applications

2. numactl's binding options work only for non-MPI applications.

3. MPI
   - MVAPICH2 supports explicit binding via MV2_CPU_MAPPING.
   - OpenMPI does not have a clear & convenient mechanism to ensure binding.

4. An application may manually bind, but this is undesirable.
