#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <utmpx.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv)
{
  int ret;
  int iam;
  int rank;
  int nranks;
  int resultlen;                   /* needed by MPI_Get_processor_name() */
  char procname[MPI_MAX_PROCESSOR_NAME];
  char str[80];                    /* string to print */
  int *coreold;
  int *corenew;
  int nthreads;
  int n;
  int corechanged;                 /* logical indicates a thread changed core its running on */
  unsigned long totsleep = 0;      /* total ms slept between core changes (init to 0) */
  const unsigned long usec = 1000; /* us to sleep: 1ms */

  void whichcore (int *);
  void printresults (char *, int, int *);

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Get_processor_name (procname, &resultlen);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nranks);

  printf ("Rank %d is running on proc %s\n", iam, procname);
  nthreads = omp_get_max_threads ();
  coreold = (int *) malloc (nthreads * sizeof (int));
  corenew = (int *) malloc (nthreads * sizeof (int));
  for (n = 0; n < nthreads; ++n)
    coreold[n] = 0;

  while (1) {
#pragma omp parallel for
    for (n = 0; n < nthreads; ++n) {
      whichcore (corenew);
    }

    corechanged = 0;
    for (n = 0; n < nthreads; ++n) {
      if (corenew[n] != coreold[n]) {
	corechanged = 1;
	break;
      }
    }

    /* Loop over ranks with a barrier inside to serialize output */
    for (rank = 0; rank < nranks; ++rank) {
      if (rank == iam) {
	if (corechanged) {
	  sprintf (str, "Rank %d results changed after %lu ms", iam, totsleep);
	  printresults (str, nthreads, corenew);
	  fflush (stdout);
	  for (n = 0; n < nthreads; ++n)
	    coreold[n] = corenew[n];
	  totsleep = 0;
	}
      }
      ret = MPI_Barrier (MPI_COMM_WORLD);
    }
    usleep (usec);
    totsleep += usec / 1000;
  }
  ret = MPI_Finalize ();
}

void whichcore (int *core)
{
  int mythread = omp_get_thread_num ();

  core[mythread] = sched_getcpu ();
}

void printresults (char *prstr, int nthreads, int *core)
{
  int n;
  
  printf ("%s\n", prstr);
  for (n = 0; n < nthreads; ++n)
    printf ("%4d ", core[n]);
  printf ("\n");
}
