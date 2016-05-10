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
    int nthreads = omp_get_num_threads();
    int tid = omp_get_thread_num();

    if (tid == 0) {
      int procs = omp_get_num_procs();
      int maxt = omp_get_max_threads();
      int inpar = omp_in_parallel();
      int dynamic = omp_get_dynamic();
      int nested = omp_get_nested();

      printf("Number of processors = %d\n", procs);
      printf("Number of threads = %d\n", nthreads);
      printf("Max threads = %d\n", maxt);
      printf("In parallel? = %d\n", inpar);
      printf("Dynamic threads enabled? = %d\n", dynamic);
      printf("Nested parallelism supported? = %d\n", nested);
    }

#pragma omp barrier

    printf("[rank=%d, tid=%d]: host=%d [rank_sz=%d, thread_sz=%d]\n",
	   rank, tid, hostid, size, nthreads);

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

  const int str_sz = 2048;
  char str[str_sz];

  int str_filled_sz = 0;
      
  int sz = snprintf(str, str_sz, "[rank=%d, tid=%d]: cpuset:", rank, tid);
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

  str[str_filled_sz] = '\n'; // must be safe
  str[str_sz - 1] = '\0';    // must be safe

  printf(str);
}

