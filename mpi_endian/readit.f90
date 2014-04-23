program readit
  implicit none
  
  include 'mpif.h'

  character(len=*), parameter :: fname = 'ninexreal8'
  real(8) :: arr(9)
  integer :: ierr
  integer :: myrank = -1

  call mpi_init (ierr)
  if (ierr /= 0) then
    write(6,*)'Error calling mpi_init'
    call mpi_abort (MPI_COMM_WORLD)
    stop 999
  end if
  call mpi_comm_rank (MPI_COMM_WORLD, myrank, ierr)

  if (myrank == 0) then
    open (unit=1, form='unformatted', status='old', file=fname, iostat=ierr)
    if (ierr /= 0) then
      write(6,*)'Error opening ',fname, ' for reading'
      stop 999
    end if
    read(1)arr
    write(6,*)'Successfully read arr=', arr
  end if
  call mpi_finalize (ierr)
  stop 0
end program readit
