//***************************************************************************
// $HeadURL$
//
// Revised by Nathan Tallent
//***************************************************************************

/******************************************************************************
* FILE: omp_getEnvInfo.c
* DESCRIPTION:
*   OpenMP Example - Get Environment Information - C/C++ Version
*   The master thread queries and prints selected environment information.
* AUTHOR: Blaise Barney  7/06
* LAST REVISED: 07/12/06
******************************************************************************/

/******************************************************************************
* FILE: omp_hello.c
* DESCRIPTION:
*   OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the master thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The master thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/

#include <omp.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define __USE_GNU
#include <sched.h>


//***************************************************************************

static void
dumpAffinity(int rank, int tid);


//***************************************************************************

int
main(int argc, char *argv[])
{

#pragma omp parallel
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
    
    printf("[rank=%d, tid=%d]: thread_sz=%d\n",  0, tid, nthreads);
    
    dumpAffinity(0, tid);
  }

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

