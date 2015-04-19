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
  int ret;

  int c;                    // for spinning
  int spin = 0;             // for spinning: default is do not
  int timework = 0;         // time a fixed amount of work
  const int totiter = 1024; // total iterations for all tasks
  int niter;                // number of iterations for this task
  float results[niter];     // results returned from dowork
  int nranks;               // size of MPI_COMM_WORLD

  void dowork (const int, float *);

  if ((ret = MPI_Init (&argc, &argv)) != 0) {
    printf ("MPI_Init failed\n");
    return -1;
  }

  (void) MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  (void) MPI_Comm_size (MPI_COMM_WORLD, &nranks);

  // begin additions for spinning
  while ((c = getopt (argc, argv, "sw")) != -1) {
    switch (c) {
    case 's':
      spin = 1;
      break;
    case 'w':
      timework = 1;
      break;
    default:
      printf ("unknown option %c\n", c);
      (void) MPI_Abort (MPI_COMM_WORLD, 1);
      break;
    }
  }

  if (spin && timework) {
    printf ("spin and timework may not both be true\n");
    (void) MPI_Abort (MPI_COMM_WORLD, 1);
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

  if (timework) {
    if (totiter % nranks != 0) {
      printf ("nranks=%d needs to divide evenly into totiter=%d\n", nranks, totiter);
      (void) MPI_Abort (MPI_COMM_WORLD, 1);
    }
    niter = totiter / nranks;
    dowork (niter, results);
  }

  MPI_Finalize ();
  return 0;
}
