program main
  use mpi
  implicit none

  integer :: nranks
  integer :: myrank
  integer :: ierr

  call mpi_init (ierr)
  call mpi_comm_size (mpi_comm_world, nranks, ierr)
  call mpi_comm_rank (mpi_comm_world, myrank, ierr)
  write(6,*)'myrank=',myrank,' of',nranks

  call print_affinity (myrank)
  call mpi_finalize (ierr)
  stop 0
end program main
