// Need to define _GNU_SOURCE sometimes to get value of CPU_SETSIZE
#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#ifdef _OPENMP
#include <omp.h>
#endif

// Punt on MAC: cannot map threads to cores
#ifdef MAC
int set_affinity_ (int *rank, 
		   int *cpn, 
		   int *nthreads, 
		   int *pin2core)
{
  printf ("set_affinity: on MAC so doing nothing\n");
  return -1;
}

#else

int set_affinity_ (int *rank,      // MPI rank of calling process
		   int *cpn,       // number of cores per node
		   int *nthreads,  // number of threads per MPI task
		   int *pin2core)  // true=>pin to single core
                                   // false=>pin to range of cores
{
  int thread = 0;
  int lo;
  int hi;
  int mincore;
  int maxcore;
  int threads_per_socket = (*cpn)/2;
  int core;
  int retval = 0;
  int loccpn = *cpn;           // for easier reading
  int locrank = *rank;         // for easier reading
  int locnthreads = *nthreads; // for easier reading
  cpu_set_t setmask;

#pragma omp parallel private (thread, core, mincore, maxcore, setmask)
  {
#ifdef _OPENMP
    thread = omp_get_thread_num ();
#endif
    if (*pin2core) {
      // Pin thread n to core n
      mincore = (locrank*locnthreads + thread) % loccpn;
      maxcore = mincore;
    } else {
      // Pin thread n to range of cores depending on how many MPI ranks there are
      mincore = (locrank*locnthreads) % loccpn;
#undef SWAP_ORDER
#ifdef SWAP_ORDER
      // Swap order: low threads to socket 1, high threads to socket 0
      if (locnthreads <= loccpn/2 && locrank % 2 == 0)
	mincore += threads_per_socket;
      else
	mincore -= threads_per_socket;
#endif
      maxcore = mincore + locnthreads - 1;
    }
    CPU_ZERO (&setmask);
    for (core = mincore; core <= maxcore; ++core)
      CPU_SET (core, &setmask);
    if (sched_setaffinity (0, sizeof (setmask), &setmask) < 0) {
      printf ("setaff: bad return from sched_setaffinity\n");
      retval = -1;
    }
  }
  return retval;
}
#endif
