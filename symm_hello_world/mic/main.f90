program main
  use mpi
  implicit none

  character(len=MPI_MAX_PROCESSOR_NAME) :: mynode
  integer :: iam
  integer :: ret
  integer :: nranks
  integer :: resultlen

  call mpi_init (ret)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ret)
  call mpi_comm_size (MPI_COMM_WORLD, nranks, ret)
  mynode = ' '
  call mpi_get_processor_name (mynode, resultlen, ret)
  write(6,100)iam, nranks, trim(mynode)
100 format ('MPI rank=',i4,' of ',i4,' host=',a)
  call mpi_finalize (ret)
end program main
