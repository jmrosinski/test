#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main (int argc, char **argv)
{
  const int mb = 1024*1024;
  long nbytes;
  char *arr;
  int ret;
  int iam;
  int size;

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &size);
  printf ("iam=%d size=%d\n", iam, size);

  for (nbytes = 1000*mb; ;nbytes *= 1.1) {
    printf ("iam %d malloc asking for %f GB...\n", iam, nbytes/(1024.*mb));
    if (arr = (char *) malloc (nbytes)) {
      printf ("iam %d malloc succeeded, now memset...\n", iam);
      memset (arr, 0, nbytes);
      printf ("iam %d Success\n", iam);
    } else {
      printf ("iam %d malloc failed. quitting.\n", iam);
      ret = MPI_Finalize ();
      exit (0);
    }
    ret = MPI_Barrier (MPI_COMM_WORLD);
    free (arr);
  }
}

