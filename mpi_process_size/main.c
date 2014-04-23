#include <mpi.h>
#include <gptl.h>

int main (int argc, char **argv)
{
  int rootid;
  int iam = -1;
  int ret;

  ret = GPTLinitialize ();
  ret = GPTLprint_memusage ("before_MPI_Init");
  ret = MPI_Init (&argc, &argv);
  ret = GPTLprint_memusage ("after_MPI_Init ");
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  if (iam == 0) 
    rootid = iam;
  ret = MPI_Bcast (&rootid, 1, MPI_INT, 0, MPI_COMM_WORLD);
  ret = GPTLprint_memusage ("after_MPI_Bcast ");
  ret = MPI_Finalize ();
}
