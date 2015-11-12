program main
  use mpi

  implicit none

  character(len=MPI_MAX_PROCESSOR_NAME) :: name
  integer :: iam
  integer :: nranks
  integer :: ierr
  integer :: resultlen
  integer :: n
  integer :: nthreads
  integer :: mythread

  integer, external :: omp_get_max_threads, omp_get_thread_num

  call mpi_init (ierr)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ierr)
  call mpi_comm_size (MPI_COMM_WORLD, nranks, ierr)
  call mpi_get_processor_name (name, resultlen, ierr)
  write(6,100) iam, nranks, name(:resultlen)
100 format ('Rank ',i3,' of ',i3,' is running on host ',a)

  nthreads = omp_get_max_threads ()
!$OMP PARALLEL DO PRIVATE (MYTHREAD)
  do n=1,nthreads
    mythread = omp_get_thread_num ()
    write(6,101) mythread, nthreads, iam
101 format ('Thread=', i3, ' of ', i3, ' rank=', i3)
  end do

  call mpi_barrier (MPI_COMM_WORLD, ierr)
  call mpi_finalize (ierr)
  stop 0
end program main
