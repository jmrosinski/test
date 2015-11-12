program main
  use mpi
  implicit none

  integer, parameter :: bytes_per_int = 4
  integer, parameter :: onemb = (1024*1024)/bytes_per_int
  integer, parameter :: niter = 1024  ! enough iterations to transfer 1GB
  integer, parameter :: tag = 99
  integer :: arr(onemb)
  character(len=MPI_MAX_PROCESSOR_NAME) :: name
  integer :: status
  integer :: iam
  integer :: nranks
  integer :: ierr
  integer :: resultlen
  integer :: n
  integer(8) :: count_rate  ! for timing (8-byte for high prec)
  integer(8) :: count_start ! for timing (8-byte for high prec)
  integer(8) :: count_end ! for timing (8-byte for high prec)

  call mpi_init (ierr)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ierr)
  call mpi_comm_size (MPI_COMM_WORLD, nranks, ierr)
  call mpi_get_processor_name (name, resultlen, ierr)
  write(6,100) iam, nranks, name(:resultlen)
100 format ('Rank ',i6,' of ',i6,' is running on host ',a)

  if (mod (nranks, 2) /= 0) then
    if (iam== 0) then
      write(6,*)'Need an even number of ranks for timing tests'
    end if
    call mpi_abort (MPI_COMM_WORLD, 1, ierr)
  end if
  call system_clock (count_rate=count_rate)

  arr(:) = iam
  call mpi_barrier (MPI_COMM_WORLD, ierr)
  call system_clock (count=count_start)
  if (mod (iam, 2) == 0) then  ! sender
    do n=1,niter
      call mpi_send (arr, onemb, MPI_INTEGER, iam+1, tag, MPI_COMM_WORLD, ierr)
    end do
  else                         ! receiver
    do n=1,niter
      call mpi_recv (arr, onemb, MPI_INTEGER, iam-1, tag, MPI_COMM_WORLD, status, ierr)
    end do
  end if
  call system_clock (count=count_end)

  if (mod (iam, 2) == 0) then  ! sender
    write(6,200) iam, iam+1, (count_end - count_start)/(1._8*count_rate)
200 format ('iam=',i6,' sent  1GB to rank=', i6,' at a rate of ',1p,e9.2,' GB/s')
  else                         ! receiver
    write(6,201) iam, iam-1, (count_end - count_start)/(1._8*count_rate)
201 format ('iam=',i6,' recvd 1GB fm rank=', i6,' at a rate of ',1p,e9.2,' GB/s')
  end if

  call mpi_finalize (ierr)
  stop
end program main
