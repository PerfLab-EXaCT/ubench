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


#define DO_PAPI 0

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
#include <cmath>

#include <stdint.h>

#include <assert.h>

#include <sched.h>

//***************************************************************************

#include <mpi.h>

#if (DO_PAPI)
# include <papi.h>
#endif

//***************************************************************************

#include "timer.h"

//***************************************************************************

static void
dumpAffinity(int myRank, int numRanks);

static void
cost_MPIWtime(int numRepeat, int myRank, int numRanks);

static void
cost_getTime(int numRepeat, int myRank, int numRanks);

static void
cost_getTSC(int numRepeat, int myRank, int numRanks);

static void
cost_PAPI(int numRepeat, int myRank, int numRanks);

static void
time_MPIAllreduce(int numRepeat, int myRank, int numRanks, bool doPerCallTime);

static void
time_MPIAllreduceData(int numRepeat, int myRank, int numRanks);


//***************************************************************************

int
main(int argc, char *argv[])
{
  bool doBenchmarks = true;
  if (argc >= 2) {
    std::string arg1 = argv[1];
    if (arg1 == "-i" || arg1 == "--info") {
      doBenchmarks = false;
    }
  }

  MPI_Init(&argc, &argv);

  int numRanks, myRank;
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  dumpAffinity(myRank, numRanks);

  if (doBenchmarks) {
    const int numRepeat = 10000;

    cost_MPIWtime(2 * numRepeat, myRank, numRanks);
    cost_getTime(2 * numRepeat, myRank, numRanks);
    cost_getTSC(3 * numRepeat, myRank, numRanks);
    cost_PAPI(2 * numRepeat, myRank, numRanks);
    
    time_MPIAllreduce(numRepeat, myRank, numRanks, false/*doPerCallTime*/);
    time_MPIAllreduce(numRepeat / 10, myRank, numRanks, true/*doPerCallTime*/);
    time_MPIAllreduceData(numRepeat / 100, myRank, numRanks);
  }

  MPI_Finalize();
  return 0;
}


static void
dumpAffinity(int myRank, int numRanks)
{
  MPI_Barrier(MPI_COMM_WORLD);

  cpu_set_t set;

  // Note: probably best to do this *before* mpi init
  //   CPU_ZERO(&set);
  //   pos = (*me % 16) * 2; // Interlagos specific
  //   CPU_SET(pos, &set);
  //   sched_setaffinity(0, sizeof(set), &set);

  CPU_ZERO(&set);
  sched_getaffinity(0, sizeof(set), &set);

  std::ostringstream os;
  os << "cpuset[rnk " << std::setw(3) << myRank << "]:";
  // std::setfill('0')

  for (size_t i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &set)) {
      os << " " << i;
    }
  }
  os << std::endl;

  cout << os.str();

  MPI_Barrier(MPI_COMM_WORLD);
}


//***************************************************************************
// 
//***************************************************************************

static void
cost_MPIWtime(int numRepeat, int myRank, int numRanks)
{
  int rc;
  MPI_Comm comm = MPI_COMM_WORLD;

  // --------------------------------------------------------
  // 
  // --------------------------------------------------------

  rc = MPI_Barrier(comm);
  assert(rc == MPI_SUCCESS);

  double timeBeg_s = MPI_Wtime();

  // N.B.: The Intel compiler (at -O3) treats MPI_Wtime as side-effect free
  double t = 0.0;
  for (size_t i = 0; i < numRepeat; i++) {
    t += MPI_Wtime();
  }

  double timeEnd_s = MPI_Wtime();

  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  if (myRank == 0) {
    MPI_Aint* isWtimeGbl_ptr = NULL;
    int isWtimeGbl_flag = 0;
    int isWtimeGbl = 0;
    
    int rc = PMPI_Comm_get_attr(comm, MPI_WTIME_IS_GLOBAL,
				&isWtimeGbl_ptr, &isWtimeGbl_flag);
    assert(rc == MPI_SUCCESS);
    isWtimeGbl = *isWtimeGbl_ptr;

    std::string sync_str = "unknown";
    if (isWtimeGbl_flag) {
      sync_str = (isWtimeGbl) ? "yes" : "no";
    }

    double time_us = (timeEnd_s - timeBeg_s) * 1e6 / (double)numRepeat;
    double res = MPI_Wtick();
    cout << "cost mpi_wtime(" << res << " s res): " << time_us << " us"
	 << " (avg. over " << numRepeat << " invocations)"
	 << " [sync: " << sync_str << "]\n";
  }
}


static void
cost_getTime(int numRepeat, int myRank, int numRanks)
{
  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  double timeBeg_s = MPI_Wtime();
  
  uint64_t t = 0;
  for (size_t i = 0; i < numRepeat; i++) {
    uint64_t tt = 0;
    time_getTimeReal(&tt);
    t += tt;
  }

  double timeEnd_s = MPI_Wtime();

  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  if (myRank == 0) {
    double time_us = (timeEnd_s - timeBeg_s) * 1e6 / (double)numRepeat;
    cout << "cost getTime(1 us res): " << time_us << " us"
	 << " (avg. over " << numRepeat << " invocations)\n";
  }
}


static void
cost_getTSC(int numRepeat, int myRank, int numRanks)
{
  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  MPI_Barrier(MPI_COMM_WORLD);
  
  double timeBeg_s = MPI_Wtime();
  
  uint64_t t = 0;
  for (size_t i = 0; i < numRepeat; i++) {
    t += time_getTSC();
  }

  double timeEnd_s = MPI_Wtime();
  
  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  if (myRank == 0) {
    double resolution = MPI_Wtick();
    double time_us = (timeEnd_s - timeBeg_s) * 1e6 / (double)numRepeat;
    cout << "cost getTSC(cycle res): " << time_us << " us"
	 << " (avg. over " << numRepeat << " invocations)\n";
  }
}


//***************************************************************************
// 
//***************************************************************************

static void
cost_PAPI(int numRepeat, int myRank, int numRanks)
{
#if (DO_PAPI)
  int ret;
  PAPI_set_debug(0x3ff);

  ret = PAPI_library_init(PAPI_VER_CURRENT);
  if (ret != PAPI_VER_CURRENT && ret > 0) {
    fprintf(stderr, "PAPI_library_init %d: %s\n", ret, PAPI_strerror(ret));
  }

  int eventSet = PAPI_NULL;
  if ( (ret = PAPI_create_eventset(&eventSet)) != PAPI_OK) {
    fprintf(stderr, "PAPI_create_eventset %d: %s\n", ret, PAPI_strerror(ret));
  }

  if ( (ret = PAPI_add_event(eventSet, PAPI_TOT_CYC)) != PAPI_OK) {
    fprintf(stderr, "PAPI_add_event1 %d: %s\n", ret, PAPI_strerror(ret));
  }

  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  const int numPapiEvnts = 1;
  long long values[numPapiEvnts];

  MPI_Barrier(MPI_COMM_WORLD);

  double timeBeg_s = MPI_Wtime();
  
  for (size_t i = 0; i < numRepeat; i++) {
    if (PAPI_start(eventSet) != PAPI_OK) {
      fprintf(stderr, "PAPI_start\n");
    }
    if (PAPI_stop(eventSet, values) != PAPI_OK) {
      fprintf(stderr, "PAPI_stop\n");
    }
  }

  double timeEnd_s = MPI_Wtime();

  // --------------------------------------------------------
  // 
  // --------------------------------------------------------
  
  if (myRank == 0) {
    double time_us = (timeEnd_s - timeBeg_s) * 1e6 / (double)numRepeat;
    cout << "cost papi start/stop: " << time_us << " us"
	 << " (avg. over " << numRepeat << " invocations)\n";
  }
#endif
}


//***************************************************************************
// 
//***************************************************************************

static void
time_MPIAllreduce(int numRepeat, int myRank, int numRanks, bool doPerCallTime)
{
  const size_t bufSzExpMax = 13;

  bool doBatchTiming = !doPerCallTime;

  for (size_t bufSzExp = 0; bufSzExp < bufSzExpMax; bufSzExp++) {
    const size_t bufSz = (size_t)pow(2, bufSzExp);
    const size_t bufSzBytes = bufSz * sizeof(double);

    double* inBuf = new double[bufSz];
    double* outBuf = new double[bufSz];
    double* solBuf = new double[bufSz];

    //double sol_scale = pow((double)numRanks, (double)numRepeat);

    for (size_t i = 0; i < bufSz; i++) {
      inBuf[i]  = 0.0; // i;
      outBuf[i] = 0.0; // 0;
      solBuf[i] = 0.0; // i * sol_scale;
    }

    // --------------------------------------------------------
    // begin timings
    // --------------------------------------------------------
    MPI_Barrier(MPI_COMM_WORLD);

    double timeBeg_s_mpi, timeEnd_s_mpi, timeSum_s_mpi = 0;
    uint64_t timeBeg_tsc, timeEnd_tsc, timeSum_tsc = 0;

    if (doBatchTiming) {
      timeBeg_s_mpi = MPI_Wtime();
      timeBeg_tsc   = time_getTSC();
    }

    // --------------------------------------------------------
    // core loop
    // --------------------------------------------------------
    for (size_t i = 0; i < numRepeat; i++) {
      if (doPerCallTime) {
	timeBeg_s_mpi = MPI_Wtime();
	timeBeg_tsc   = time_getTSC();
      }

      MPI_Allreduce(inBuf, outBuf, bufSz, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

      if (doPerCallTime) {
	timeEnd_s_mpi = MPI_Wtime();
	timeEnd_tsc   = time_getTSC();
	timeSum_s_mpi += (timeEnd_s_mpi - timeBeg_s_mpi);
	timeSum_tsc   += (timeEnd_tsc - timeBeg_tsc);
      }
    }

    // --------------------------------------------------------
    // end timings
    // --------------------------------------------------------

    if (doBatchTiming) {
      timeEnd_s_mpi = MPI_Wtime();
      timeEnd_tsc   = time_getTSC();
      timeSum_s_mpi += (timeEnd_s_mpi - timeBeg_s_mpi);
      timeSum_tsc   += (timeEnd_tsc - timeBeg_tsc);
    }

    if (myRank == 0) {
      double time_us_mpi, time_us_tsc = 0.0;

      time_us_mpi = timeSum_s_mpi * 1e6 / (double)numRepeat;
      time_us_tsc = time_TSC2us(timeSum_tsc) / (double)numRepeat;

      const char* t_str = (doPerCallTime) ? "per-call" : "batch";

      cout << "mpi_allreduce(" << std::setw(5) << bufSzBytes << " bytes): "
	   << time_us_mpi << " us (MPI_Wtime); "
	   << time_us_tsc << " us (TSC " << MY_CPU_MHZ << ") "
	   << "[" << t_str << " times, " << numRepeat << " invocations]\n";
    }

    // --------------------------------------------------------
    // correctness
    // --------------------------------------------------------
    for (size_t i = 0; i < bufSz; i++) {
      if (outBuf[i] != solBuf[i]) {
	cerr << "Error at position " << i << "\n";
      }
    }

    delete[] inBuf;
    delete[] outBuf;
    delete[] solBuf;
  }
}


static void
time_MPIAllreduceData(int numRepeat, int myRank, int numRanks)
{
  const size_t dataSz = (size_t)pow(2, 21);
  double* data = new double[dataSz];
  double sign = 1;
  for (size_t i = 0; i < dataSz; i++) {
    data[i] = sign * (i * numRanks);
    sign *= -1;
  }

  const size_t bufSz = 4;
  const size_t bufSzBytes = bufSz * sizeof(double);

  double* inBuf = new double[bufSz];
  double* outBuf = new double[bufSz];
  double* solBuf = new double[bufSz];

  for (size_t i = 0; i < bufSz; i++) {
    inBuf[i] = i;
    outBuf[i] = 0;
    solBuf[i] = i * numRanks;
  }


  double timeCpu_comp_sum_us = 0;
  double time_comp_sum_us = 0;
  double time_comm_sum_us = 0;

  for (size_t iter = 0; iter < numRepeat; iter++) {
    // synchronize and collect begin time

    int rank1 = iter % numRanks;
    int rank2 = (iter + 7) % numRanks;

    MPI_Barrier(MPI_COMM_WORLD);

    uint64_t timeCpu1_us;
    time_getTimeCPU(&timeCpu1_us);

    double time1_s = MPI_Wtime();

    // --------------------------------------------------------
    // compute
    // --------------------------------------------------------
    double val = 0;
    for (size_t i = 0; i < dataSz; i++) {
      val += data[i];
    }
    // --------------------------------------------------------

    uint64_t timeCpu2_us;
    time_getTimeCPU(&timeCpu2_us);

    double time2_s = MPI_Wtime();

    // --------------------------------------------------------
    // allreduce
    // --------------------------------------------------------
    MPI_Allreduce(inBuf, outBuf, bufSz, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    // --------------------------------------------------------

    double time3_s = MPI_Wtime();

    double timeCpu_comp_us  = (double)(timeCpu2_us - timeCpu1_us);
    double time_comp_us     = (time2_s - time1_s) * 1e6;
    double time_comm_us     = (time3_s - time2_s) * 1e6;
    double time_compDiff_us = time_comp_us - timeCpu_comp_us;

    timeCpu_comp_sum_us += timeCpu_comp_us;
    time_comp_sum_us    += time_comp_us;
    time_comm_sum_us    += time_comm_us;

    if (myRank == rank1 || myRank == rank2) {
      cout << "comp(usr,real(diff));allreduce(" << bufSzBytes << " bytes)["
	   << std::setw(3) << iter << "/" << std::setw(3) << myRank << "] us: "
	   << timeCpu_comp_us << ", " << time_comp_us
	   << " (" << time_compDiff_us << "); " << time_comm_us
	   << " [val: " << val << "]\n";
    }
  }

  if (myRank == 0 || myRank == 7) {
    double timeCpu_comp_avg_us  = timeCpu_comp_sum_us / (double)numRepeat;
    double time_comp_avg_us     = time_comp_sum_us / (double)numRepeat;
    double time_comm_avg_us     = time_comm_sum_us / (double)numRepeat;
    double time_compDiff_avg_us = time_comp_avg_us - timeCpu_comp_avg_us;

    cout << "comp(usr,real(diff));allreduce(" << bufSzBytes << " bytes)[*/"
	 << myRank << "] us: "
	 << timeCpu_comp_avg_us << ", " << time_comp_avg_us
	 << " (" << time_compDiff_avg_us << "); " << time_comm_avg_us
	 << " (avg. over " << numRepeat << " invocations)]\n";
  }

  delete[] inBuf;
  delete[] outBuf;
  delete[] solBuf;
  
  delete[] data;
}
