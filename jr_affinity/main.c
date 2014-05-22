#include <stdio.h>
#include <unistd.h>  // gethostname, getopt
#include <sched.h>
#include <mpi.h>
#include <omp.h>

extern void runnable (cpu_set_t *, int *, int *);

int main (int argc, char **argv)
{
  char hnbuf[64];
  int rank;
  int thread;
  int lo;
  int hi;
  cpu_set_t coremask;

  int c;        // for spinning
  int spin = 0; // for spinning: default is do not

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);

  // begin additions for spinning
  while ((c = getopt (argc, argv, "s")) != -1) {
    switch (c) {
    case 's':
      spin = 1;
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
      break;
    }
  }

  (void) gethostname (hnbuf, sizeof (hnbuf));
#pragma omp parallel private (thread, coremask)
  {
    thread = omp_get_thread_num ();
    // Passing zero means use the calling process
    (void) sched_getaffinity (0, sizeof (coremask), &coremask);
    runnable (&coremask, &lo, &hi);
    #pragma omp barrier
    printf ("Rank %d thread %d on %s. (Runnable range: lo=%d hi=%d)\n",
	    rank, thread, hnbuf, lo, hi);
    // Put in a spin loop
    if (spin)
      while (1);
  }
  MPI_Finalize ();
  return 0;
}
