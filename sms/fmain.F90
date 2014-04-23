program main
  use decomp
!SMS$IGNORE BEGIN
  use mpi
!SMS$IGNORE END

  implicit none

  integer :: ipn

!SMS$SET_COMMUNICATOR (MPI_COMM_WORLD)

  call driver ()
  stop
end program main
