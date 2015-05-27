#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  int nranks;
  int iam;
  int ret;
  int retcode;
  char *val;

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nranks);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);

  if (val = getenv ("ENVVAR")) {
    retcode = 0;
    if (iam == 1)
      printf ("Success: ENVVAR=%s\n", val);
  } else {
    retcode = 1;
    if (iam == 1)
      printf ("Failure: value for ENVVAR not found\n");
  }
  ret = MPI_Finalize ();
  return retcode;
}
