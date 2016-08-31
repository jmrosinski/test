#include <stdio.h>
#include <unistd.h> // getopt
#include <stdlib.h> // atoi, getenv
#include <mpi.h>
#include <omp.h>

int sub (int, unsigned long, char);

int main (int argc, char **argv)
{
  int ret;
  int iam;                // my rank
  int c;                  // for parsing arg list
  int n;                  // iterator
  int nthreads = 1;       // number of threads (default 1)
  const float frac = 1.1; // increase stacksize by 10% each iteration
  unsigned long initstack = 1000000;
  unsigned long stacksize;
  char *kmp_stacksize;

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  printf ("Rank %d is running\n", iam);

  while ((c = getopt (argc, argv, "t:")) != -1) {
    switch (c) {
    case 't':
      if ((nthreads = atoi (optarg)) < 1) {
	printf ("-t nthreads must be > 0\n");
	return 1;
      }
      break;
    default:
      if (iam == 0) {
	printf ("unknown option %c\n", c);
	printf ("Usage: <mpiruncmd> %s [-t <nthreads>]\n", argv[0]);
	return 1;
      }
    }
  }

  omp_set_num_threads (nthreads);
  nthreads = omp_get_max_threads ();
  kmp_stacksize = getenv ("KMP_STACKSIZE");
  printf ("iam=%d nthreads=%d KMP_STACKSIZE=%s\n", iam, nthreads, kmp_stacksize);
  
  for (stacksize = initstack; ; stacksize *= frac) {
#pragma omp parallel for private (n, ret)
    for (n = 0; n < nthreads; ++n) {
      ret = sub (iam, stacksize, 'x');
    }
  }
  ret = MPI_Finalize ();
  return 0;
  }

int sub (int iam, unsigned long stacksize, char val)
{
  char arr[stacksize];
  unsigned long n;
  int mythread = omp_get_thread_num ();
  printf ("Start sub iam=%d mythread=%d stacksize=%g\n", 
	  iam, mythread, stacksize/1.e6);
  for (n = 0; n < stacksize; ++n) {
    arr[n] = 'y';
  }

  for (n = 0; n < stacksize; ++n) {
    if (arr[n] != val)
      break;
  }

  if (n == stacksize) {
    printf ("n should never = stacksize!\n");
    return 1;
  }

  printf ("sub successful return stacksize=%g\n", stacksize/1.e6);
  return 0;
}
