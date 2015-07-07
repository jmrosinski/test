#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  int nranks;
  int iam;
  int ret;
  int retcode;
  int resultlen;
  char *val;
  char nodename[128];

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nranks);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Get_processor_name (nodename, &resultlen);

  printf ("MPI rank %d is running on node=%s\n", iam, nodename);

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
