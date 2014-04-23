program main
    include 'mpif.h'

    integer ret
    integer iam
    integer namelen

    integer :: junk=11
    integer :: junkt
    integer :: ierr
    integer stat (MPI_STATUS_SIZE)
    integer left,right

    character(len=256):: name

    name = ' '

    call MPI_Init (ret)
    call MPI_Comm_rank (MPI_COMM_WORLD, iam, ret)
    write(6,*) "Hello from mpi task number ", iam

    call MPI_Get_processor_name (name, namelen, ret)
    write(6,*) "Task ", iam, " is running on proc ", name

    left = MPI_PROC_NULL
    right = MPI_PROC_NULL
    call mpi_sendrecv (junk, 1, MPI_INTEGER, right, 1, &
                       junkt, 1, MPI_INTEGER, left, 1, MPI_COMM_WORLD, stat, ierr)
    call MPI_Finalize (ret)
end program main
