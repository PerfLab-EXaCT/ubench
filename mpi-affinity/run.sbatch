#!/bin/sh
# -*-Mode: sh;-*-

#****************************************************************************
# $HeadURL: https://svn.pnl.gov/svn/hpcgroup/tallent/mybench/mpi-affinity/run.sbatch $
# $Id: run.sbatch 12325 2015-12-07 21:43:12Z tallent $
#
# Nathan Tallent
#****************************************************************************

#SBATCH -J mpi_affinity # job name
#SBATCH -t 00:10:00 # time
#SBATCH -o run-mpi_affinity-%j.out
#SBATCH -e run-mpi_affinity-%j.out
#SBATCH -N 1 # nodes
#SBATCH --exclusive
#SBATCH --export=ALL # environment

#****************************************************************************

# mvapich2 | openmpi
mpity=mvapich2 # ${PNNL_MPI}

mpiver=${PNNL_MPI_VERSION}

mpinm=${mpity}-${mpiver:=""}

nrank="-n 4"
rankmap1="0,8,16,24" # slurm syntax
rankmap2="0:8:16:24" # mvapich syntax
cpuPerRank="4"

mv2_launch="mpiexec"

#nrank2="-n 8"
#rankmap2_a="0,4,8,12,16,20,24,28"
#rankmap2_b="0:4:8:12:16:20:24:28"
#cpuPerRank2="2"

divider="****************************************************************************"

sort="sort -g"

A1="cat /proc/self/status"
A2="grep Cpus_allowed_list"
A3="${sort}"

B1="./mpi-affinity"
B2="${sort}"


#****************************************************************************
# 
#****************************************************************************

printf "${divider}\n"
printf "Test MPI '${mpinm}'\n"
printf "${divider}\n"

printf "\n* goal: bind '${nrank}' ranks to cores ${rankmap2}\n"

printf "\n* numa info *\n"
numactl --hardware | grep cpu

#****************************************************************************

printf "\n${divider}\n"
printf "1. srun [expect: good with non-mpi app]\n"
printf "${divider}\n"

#verbose="verbose,"

printf "\n* srun/non-mpi/hint=compute_bound [expect: bad]\n"
srun ${nrank} --hint=compute_bound $A1 | $A2 | $A3

printf "\n* srun/non-mpi/hint=memory_bound: [expect: bad]\n"
srun ${nrank} --hint=memory_bound $A1 | $A2 | $A3

printf "\n* srun/non-mpi/bind=cores [expect: bad]\n"
srun ${nrank} --cpu_bind=${verb}cores $A1 | $A2 | $A3

printf "\n* srun/non-mpi/bind=map:${rankmap1} [expect: good]\n"
srun ${nrank} --cpu_bind=${verb}map_cpu:${rankmap1} $A1 | $A2 | $A3

if test "${mpity}" = "mvapich2" ; then
  printf "\n* srun/mpi/bind=map:${rankmap1} [expect: bad]\n"
  srun ${nrank} --cpu_bind=${verb}map_cpu:${rankmap1} $B1 | $B2
fi

#****************************************************************************

printf "\n${divider}\n"
printf "2. srun+numactl [expect: ok with non-mpi app]\n"
printf "${divider}\n"

printf "\n* srun+numactl/non-mpi/bind=${rankmap1} [expect: ok]\n"
srun ${nrank} numactl --physcpubind=${rankmap1} $A1 | $A2 | $A3

if test "${mpity}" = "mvapich2" ; then
  printf "\n* srun+numactl/mpi/bind=${rankmap1} [expect: bad]\n"
  srun ${nrank} numactl --physcpubind=${rankmap1} $B1 | $B2
fi

#****************************************************************************

if test "${mpity}" = "mvapich2" ; then
    
    
  printf "\n${divider}\n"
  printf "3. ${mpinm} (via ${mv2_launch}) [expect: good with explicit mapping]\n"
  printf "${divider}\n"

  printf "\n* mvapich/mpi/(default) [expect: bad]\n"
  ${mv2_launch} ${nrank} $B1 | $B2

  printf "\n* mvapich/mpi/affinity [expect: bad]\n"
  MV2_ENABLE_AFFINITY=1 ${mv2_launch} ${nrank} $B1 | $B2

  printf "\n* mvapich/mpi/map-${rankmap2} [expect: good]\n"
  MV2_CPU_MAPPING=${rankmap2} ${mv2_launch} ${nrank} $B1 | $B2

fi

#****************************************************************************

if test "${mpity}" = "openmpi" ; then

  printf "\n${divider}\n"
  printf "3. ${mpinm} [expect: 1.5 bind to module; 1.6 ok slot-list; 1.7 ok cpus-per-rank]\n"
  printf "${divider}\n"

  #verb="--report-bindings"

  printf "\n* mpirun/mpi/default [expect: bad]\n"
  mpirun ${nrank} ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/cpus-per-rank: [expect: 1.5 ok; 1.6 ok; 1.7 ok]\n"
  mpirun ${nrank} --cpus-per-rank ${cpuPerRank} ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/bind-core [expect: 1.5 ok/bind-module; 1.6 ok; 1.7 bad]\n"
  mpirun ${nrank} --bind-to-core ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/bind-socket [expect: 1.5 bad; 1.6 bad; 1.7 bad]\n"
  mpirun ${nrank} --bind-to-socket ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/bind-numa [expect: 1.5 fail; 1.6 fail; 1.7 ?]\n"
  mpirun ${nrank} --bind-to numa ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/cpu-set=${rankmap1}: [expect: 1.5 bad; 1.6 fail; 1.7 ok]\n"
  mpirun ${nrank} --cpu-set ${rankmap1} ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/slot-list=${rankmap1} [expect: 1.5 fail; 1.6 ok; 1.7 fail]\n"
  mpirun ${nrank} --slot-list ${rankmap1} ${verb} $B1 | $B2

  printf "\n* mpirun/mpi/rankfile [???]\n"
  # --rankfile <rankfile> ???

fi

#****************************************************************************

printf "\n${divider}\n"
printf "Info\n"
printf "${divider}\n\n"

printf "*** nodes: ${SLURM_JOB_NODELIST} ***\n\n"
printf '*** modules ***\n'     && module list -l 2>&1 && printf '\n'
printf '*** limits ***\n'      && ulimit -a && printf '\n'
printf '*** environment ***\n' && env       && printf '\n'

