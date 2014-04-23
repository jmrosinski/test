program main
  use mpi
  implicit none

  character(len=MPI_MAX_PROCESSOR_NAME) :: nodename
  integer :: iam
  integer :: ntasks
  integer :: ret
  integer :: resultlen

  call mpi_init (ret)
  call mpi_comm_size (MPI_COMM_WORLD, ntasks, ret)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ret)
  call mpi_get_processor_name (nodename, resultlen, ret)
  write(6,*)'MPI rank ', iam, ' of ', ntasks, ' is running on ', trim(nodename)
  call mpi_finalize (ret)
  stop
end program main
