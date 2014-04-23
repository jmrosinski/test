#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char **argv)
{
  int c;
  int ret;             // return code
  int iam;             // MPI task number
  int namelen;         // length returned from MPI_Get_processor_name
  int resultlen;
  int inc = 1;
  int i = 0;
  int masterproc = -1;
  int size = -1;       // size of MPI_COMM_WORLD
  int left;            // proc logically to my left
  int right;           // proc logically to my right
  int sendval;         // for MPI_Sendrecv
  int recvval;         // for MPI_Sendrecv
  
  long looplen = 100000;

  int *a = NULL;

  double sum;
  double global_sum;  // For MPI_Allreduce

  char name[MPI_MAX_PROCESSOR_NAME]; // processor name
  char string[MPI_MAX_ERROR_STRING];

  MPI_Status stat;

  extern double sumsub (int, long);
  extern double sinsub (int, long);
  extern double expsub (int, long);
  extern double logsub (int, long);
  extern double sqrtsub (int, long);

  while ((c = getopt (argc, argv, "l:")) != -1) {
    switch (c) {
    case 'l':
      looplen = atol (optarg);
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
      break;
    }
  }

  memset (name, 0, MPI_MAX_PROCESSOR_NAME);

  if ((ret = MPI_Init (&argc, &argv)) != MPI_SUCCESS)
    error ("Failure in MPI_Init\n");
  
  if ((ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam)) != MPI_SUCCESS)
    error ("Failure in MPI_Comm_rank\n");

  if ((ret = MPI_Comm_size (MPI_COMM_WORLD, &size)) != MPI_SUCCESS)
    error ("Failure in MPI_Comm_size\n");

  if (iam == 0) {
    printf ("loop length is: %d\n", looplen);
    masterproc = iam;
  }

  if ((ret = MPI_Get_processor_name (name, &namelen)) != MPI_SUCCESS)
    error ("Failure in MPI_Get_processor_name\n");

  printf ("Task %d process name %s is running on the processor named %s\n", iam, argv[0], name);
  fflush (stdout);
  
  sum = sumsub (iam, looplen);
  sum = sinsub (iam, looplen);
  sum = expsub (iam, looplen);
  sum = logsub (iam, looplen);
  sum = sqrtsub (iam, looplen);
 
  // Send to the left, receive from the right

  left  = iam == 0        ? size - 1 : iam - 1;
  right = iam == size - 1 ? 0        : iam + 1;
      
  for (i = 0; i < looplen; ++i) {
    ret = MPI_Allreduce (&sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    ret = MPI_Bcast (&masterproc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    sendval = iam;
    recvval = -1;
    ret = MPI_Sendrecv (&sendval, 1, MPI_INT, right, 1,
			&recvval, 1, MPI_INT, left, 1, MPI_COMM_WORLD, &stat);
    if (recvval != left) {
      MPI_Abort (MPI_COMM_WORLD, 1);
      return 1;  // In case MPI_Abort fails
    }
  }
  
  if ((ret = MPI_Finalize ()) != MPI_SUCCESS) {
    MPI_Error_string (ret, string, &resultlen);
    error ("Failure in MPI_Finalize: %s\n", string);
  }
  return 0;
}
