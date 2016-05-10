//***************************************************************************
// $HeadURL$
//
// Revised by Nathan Tallent
//***************************************************************************

/******************************************************************************
* FILE: omp_mm.c
* DESCRIPTION:  
*   OpenMp Example - Matrix Multiply - C Version
*   Demonstrates a matrix multiply using OpenMP. Threads share row iterations
*   according to a predefined chunk size.
* AUTHOR: Blaise Barney
* LAST REVISED: 06/28/05
******************************************************************************/

#include <mpi.h>

#include <omp.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/resource.h>

//#define DO_PRINT

#ifdef DO_SMALL

#define NRA 62                 /* number of rows in matrix A */
#define NCA 15                 /* number of columns in matrix A */
#define NCB 7                  /* number of columns in matrix B */

#else

#define NRA 6000              /* number of rows in matrix A */
#define NCA 6568              /* number of columns in matrix A */
#define NCB 6255              /* number of columns in matrix B */

#endif

int thread_main(int rank);


int
main(int argc, char *argv[])
{
  struct rlimit lim;

  if (getrlimit(RLIMIT_STACK, &lim) != 0) {
    printf("error: getrlimit()\n");
    return -1;
  }

  lim.rlim_cur = RLIM_INFINITY;
  
  if (setrlimit(RLIMIT_STACK, &lim) != 0) {
    printf("error: setrlimit()\n");
    return -1;
  }

  MPI_Comm comm = MPI_COMM_WORLD;
  int size, rank;
    
  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  
  int ret = thread_main(rank);

  MPI_Finalize();

  return ret;
}


int
thread_main(int rank)
{
  int tid, nthreads, i, j, k, chunk;

  double a[NRA][NCA];           /* matrix A to be multiplied */
  double b[NCA][NCB];           /* matrix B to be multiplied */
  double c[NRA][NCB];           /* result matrix C */

#ifdef DO_SMALL
  chunk = 10;                   /* set loop iteration chunk size */
#else
  chunk = 200;                  /* set loop iteration chunk size */
#endif

#pragma omp parallel shared(rank,a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
    tid = omp_get_thread_num();
    if (tid == 0) {
      nthreads = omp_get_num_threads();
      printf("[rank=%d] starting with %d threads...\n", rank, nthreads);
    }

    // ----------------------------------------
    // initialize matrices
    // ----------------------------------------    
#pragma omp for schedule (static, chunk) 
    for (i=0; i<NRA; i++)
      for (j=0; j<NCA; j++)
	a[i][j]= i+j;
    
#pragma omp for schedule (static, chunk)
    for (i=0; i<NCA; i++)
      for (j=0; j<NCB; j++)
	b[i][j]= i*j;
    
#pragma omp for schedule (static, chunk)
    for (i=0; i<NRA; i++)
      for (j=0; j<NCB; j++)
	c[i][j]= 0;

    // ----------------------------------------    
    // matrix multiply
    // ----------------------------------------    
    printf("[rank=%d, tid=%d] starting matrix multiply...\n", rank, tid);
#pragma omp for schedule (static, chunk)
    for (i=0; i<NRA; i++) {
#ifdef DO_PRINT
      printf("[rank=%d, tid=%d] did row=%d\n", rank, tid, i);
#endif
      for(j=0; j<NCB; j++)       
	for (k=0; k<NCA; k++)
	  c[i][j] += a[i][k] * b[k][j];
    }
  }

  // ----------------------------------------    
  // print results
  // ----------------------------------------    
#ifdef DO_SMALL
  printf("******************************************************\n");
  printf("Result Matrix:\n");
  for (i=0; i<NRA; i++) {
    for (j=0; j<NCB; j++) 
      printf("%6.2f   ", c[i][j]);
    printf("\n"); 
  }
  printf("******************************************************\n");
  printf ("Done.\n");
#endif
  
  return 0;
}
