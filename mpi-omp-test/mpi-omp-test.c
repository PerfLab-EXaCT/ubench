//***************************************************************************
// $HeadURL$
//
// Nathan Tallent
//***************************************************************************

#include <mpi.h>

#include <omp.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define __USE_XOPEN_EXTENDED // for gethostid()
#include <unistd.h>

#define __USE_GNU
#include <sched.h>


//***************************************************************************

#define MY_NM "mpi-omp-test"

static void
dumpAffinity(int rank, int tid);


//***************************************************************************

int
main(int argc, char *argv[])
{
  int size, rank;
  
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  
  int hostid = (int)gethostid(); // returns a 32-bit id in a long

#pragma omp parallel shared(hostid)
  {
    int n_threads = omp_get_num_threads();
    int tid = omp_get_thread_num();

    if (tid == 0) {
      int n_procs = omp_get_num_procs();
      int n_threads_max = omp_get_max_threads();
      int is_parallel = omp_in_parallel();
      int is_dynamic = omp_get_dynamic();
      int may_nest = omp_get_nested();

      printf(MY_NM "[rank=%d, tid=%d]: OMP num procs: %d\n", rank, tid, n_procs);
      printf(MY_NM "[rank=%d, tid=%d]: OMP num threads: %d\n", rank, tid, n_threads);
      printf(MY_NM "[rank=%d, tid=%d]: OMP max threads: %d\n", rank, tid, n_threads_max);
      printf(MY_NM "[rank=%d, tid=%d]: OMP in parallel? %d\n", rank, tid, is_parallel);
      printf(MY_NM "[rank=%d, tid=%d]: OMP dynamic enabled? %d\n", rank, tid, is_dynamic);
      printf(MY_NM "[rank=%d, tid=%d]: OMP nested parallelism supported? %d\n", rank, tid, may_nest);
    }

#pragma omp barrier

    printf(MY_NM "[rank=%d, tid=%d]: host=%d [rank_sz=%d, thread_sz=%d]\n",
	   rank, tid, hostid, size, n_threads);

    dumpAffinity(rank, tid);
  }
  
  MPI_Finalize();
  
  return 0;
}


static void
dumpAffinity(int rank, int tid)
{
  cpu_set_t set;
  CPU_ZERO(&set);
  sched_getaffinity(0, sizeof(set), &set);

  const int str_sz_max = 2048;
  const int str_sz = (str_sz_max - 1); // reserve space for trailing '\n'
  char str[str_sz_max];

  int str_filled_sz = 0;
      
  int sz = snprintf(str, str_sz, MY_NM "[rank=%d, tid=%d]: cpuset:", rank, tid);
  str_filled_sz += sz;
  assert(str_filled_sz < str_sz); 

  for (size_t i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &set)) {
      int str_unfilled_sz = (str_sz - str_filled_sz);
      sz = snprintf(&str[str_filled_sz], str_unfilled_sz, " %d", i);
      str_filled_sz += sz;
      assert(str_filled_sz < str_sz);
    }
  }

  str[str_filled_sz]   = '\n'; // must be safe
  str[str_filled_sz+1] = '\0'; // must be safe

  printf(str);
}

