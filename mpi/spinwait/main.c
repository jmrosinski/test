#include <string.h>
#include <unistd.h>
#include <mpi.h>

int main (int argc, char **argv)
{
  int ret;
  int size;
  int iam;
  char name[MPI_MAX_PROCESSOR_NAME];
  int resultlen;
  const size_t tenmb = 10*1024*1024;
  char *arr;

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &size);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  ret = MPI_Get_processor_name (name, &resultlen);
  printf ("MPI rank %d is running on %s\n", iam, name);
  ret = MPI_Barrier (MPI_COMM_WORLD);
  arr = (char *) malloc (tenmb);
  memset (arr, 0, tenmb);
  if (iam == 0) {
    ret = sleep (60);
  }
  ret = MPI_Finalize ();
  return 0;
}
