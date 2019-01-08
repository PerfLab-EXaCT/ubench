// -*-Mode: C++;-*- // technically C99

//*BeginPNNLCopyright********************************************************
//
// $HeadURL$
// $Id$
//
//**********************************************************EndPNNLCopyright*

//***************************************************************************
// $HeadURL$
//
// Nathan Tallent
//***************************************************************************

//***************************************************************************
// 
//***************************************************************************

#define __STDC_LIMIT_MACROS

#include <iostream>
using std::cout;
using std::cerr;

#include <sstream>

#include <iomanip>

#include <cstdlib>

#include <stdint.h>

#ifndef __USE_GNU
# define __USE_GNU
#endif
#include <sched.h>

//***************************************************************************

#include <mpi.h>

//***************************************************************************

#define MY_NM "mpi-affinity"

static void
dumpAffinity(int myRank, int numRanks, MPI_Comm comm);

//***************************************************************************
// 
//***************************************************************************

int
main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  int numRanks, myRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  dumpAffinity(myRank, numRanks, MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}

// Setting affinity: do *before* mpi init
//   CPU_ZERO(&set);
//   pos = (*me % 16) * 2; // Interlagos specific
//   CPU_SET(pos, &set);
//   sched_setaffinity(0, sizeof(set), &set);

static void
dumpAffinity(int myRank, int numRanks, MPI_Comm comm)
{
  MPI_Barrier(comm);

  cpu_set_t set;
  CPU_ZERO(&set);
  sched_getaffinity(0, sizeof(set), &set);

  std::ostringstream os;
  os << MY_NM "[rank=" << std::setw(3) << myRank << ", size=" << numRanks << "] cores";
  // std::setfill('0')

  for (size_t i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &set)) {
      os << " " << i;
    }
  }
  os << std::endl;

  cout << os.str();

  MPI_Barrier(comm);
}
