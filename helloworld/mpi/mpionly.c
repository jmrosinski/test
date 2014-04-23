#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char **argv)
{
    int ret;             // return code
    int iam;             // MPI task number
    int namelen;         // length returned from MPI_Get_processor_name

    char name[MPI_MAX_PROCESSOR_NAME]; // processor name

    memset (name, 0, MPI_MAX_PROCESSOR_NAME);

    if ((ret = MPI_Init (&argc, &argv)) != MPI_SUCCESS)
	error ("Failure in MPI_Init\n");
    
    if ((ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam)) != MPI_SUCCESS)
	error ("Failure in MPI_Comm_rank\n");

    printf ("Hello from mpi task number %d\n", iam);

    if ((ret = MPI_Get_processor_name (name, &namelen)) != MPI_SUCCESS)
	error ("Failure in MPI_Get_processor_name\n");
    
    printf ("Task %d is running on the processor named %s\n", iam, name);

    if ((ret = MPI_Finalize ()) != MPI_SUCCESS)
	error ("Failure in MPI_Finalize\n");
}
