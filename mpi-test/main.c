//***************************************************************************
// $HeadURL$
//
// Revised by Nathan Tallent
//***************************************************************************

#include <mpi.h>

#include <stdio.h>
#include <assert.h>

//#define __USE_XOPEN_EXTENDED // for gethostid()
#include <unistd.h>

//***************************************************************************

#define MY_NM "mpi-test"

//***************************************************************************

int
main(int argc, char **argv)
{
  int size, rank;
  
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);
  

  MPI_Aint* isWtimeGbl_ptr = NULL;
  int isWtimeGbl_flag = 0;
  int isWtimeGbl = 0;

  int rc = MPI_Comm_get_attr(comm, MPI_WTIME_IS_GLOBAL,
			     &isWtimeGbl_ptr, &isWtimeGbl_flag);
  assert(rc == MPI_SUCCESS);
  isWtimeGbl = *isWtimeGbl_ptr;


  MPI_Aint* tag_ub_ptr = NULL;
  int tag_ub_flag = 0;
  int tag_ub = 0;

  rc = MPI_Comm_get_attr(comm, MPI_TAG_UB, &tag_ub_ptr, &tag_ub_flag);
  assert(rc == MPI_SUCCESS);
  tag_ub = *tag_ub_ptr;

  int hostid = (int)gethostid(); // returns a 32-bit id in a long
  
  printf(MY_NM "[rank=%d, size=%d] host=%d; wtime-gbl exist?=%d val=%ld; tag_ub exist?=%d val=%d\n",
	 rank, size, hostid,
	 isWtimeGbl_flag, isWtimeGbl,
	 tag_ub_flag, tag_ub);
  
  MPI_Finalize();
  
  return (0);
}
