// Need to define _GNU_SOURCE sometimes to get value of CPU_SETSIZE
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>  // gethostname, getopt
#include <sched.h>
#include <omp.h>

extern void runnable (cpu_set_t *, int *, int *);

int main (int argc, char **argv)
{
  char hnbuf[64];
  int rank;
  int thread;
  int lo;
  int hi;
  cpu_set_t setmask;
  cpu_set_t getmask;

  int c;        // for spinning
  int spin = 0; // for spinning: default is do not

  (void) gethostname (hnbuf, sizeof (hnbuf));
#pragma omp parallel private (thread, setmask, getmask, lo, hi)
  {
    thread = omp_get_thread_num ();
    // Set affinity so thread n runs on core n
    CPU_ZERO (&setmask);
    CPU_SET (thread, &setmask);
    if (sched_setaffinity (0, sizeof (setmask), &setmask) < 0)
      printf ("setaff: bad return from sched_setaffinity\n");

    // Passing zero means use the calling process
    (void) sched_getaffinity (0, sizeof (getmask), &getmask);
    runnable (&getmask, &lo, &hi);
#pragma omp barrier
    printf ("Rank %d thread %d on %s. (Runnable range: lo=%d hi=%d)\n",
	    rank, thread, hnbuf, lo, hi);
    // Put in a spin loop
    if (spin)
      while (1);
  }
  return 0;
}
