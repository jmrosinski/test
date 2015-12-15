program main
  use mpi
  implicit none

  integer :: nranks
  integer :: myrank
  integer :: ierr, ret
  integer :: cpn, nthreads, pin2core
  character(len=1) :: ans

  integer, external :: omp_set_num_threads

  call mpi_init (ierr)
  call mpi_comm_size (mpi_comm_world, nranks, ierr)
  call mpi_comm_rank (mpi_comm_world, myrank, ierr)
  write(6,*)'myrank=',myrank,' of',nranks

  if (myrank == 0) then
    write(6,*)'Enter "y" to call set_affinity after entering cpn nthreads pin2core by hand:'
    read(5,*) ans
    if (ans == 'y') then
      write(6,*)'Enter cpn'
      read(5,*) cpn
      write(6,*)'Enter nthreads'
      read(5,*) nthreads
      write(6,*)'Enter pin2core (0=false, anything else=true)'
      read(5,*) pin2core
    end if
  end if

  call mpi_bcast (ans, 1, MPI_CHARACTER, 0, MPI_COMM_WORLD, ierr)
  call mpi_bcast (cpn, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)
  call mpi_bcast (nthreads, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)
  call mpi_bcast (pin2core, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, ierr)

  if (ans == 'y') then
    ret = omp_set_num_threads (nthreads)
    write(6,*)'Calling set_affinity(myrank=',myrank,', cpn=',cpn,', nthreads=',nthreads,', pin2core=',pin2core,')'
    call set_affinity (myrank, cpn, nthreads, pin2core)
  end if

  write(6,*)'myrank=',myrank,' Calling print_affinity'
  call print_affinity (myrank)
  call mpi_finalize (ierr)
  stop
end program main
