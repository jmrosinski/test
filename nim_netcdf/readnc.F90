program readnc
  use mpi
  use netcdf
  use gptl
  implicit none

! Model grid variables
  integer :: glvl = -1
  integer :: nip = -1
  integer :: nz = -1
  integer :: numleft
  integer :: ranksleft
  integer :: ipn, k, t
  integer, allocatable :: nipn(:), begipn(:), endipn(:)
! For future use of argument list parsing
  integer :: nargs
  integer :: n
! GPTL
  integer :: ret
! MPI
  integer :: ierr
  integer :: nranks
  integer :: iam
  integer :: resultlen
  character(len=MPI_MAX_PROCESSOR_NAME) :: procname
! Netcdf
  integer :: ncid  ! file id
  integer :: nip_dimid, nipid
  integer :: nz_dimid, nzid
  integer :: time_dimid, timeid
  integer :: varid
  integer :: dimids(3)  ! enough dims to hold nip, nz, time
  integer :: start(3), kount(3)
  integer :: ntime
  character(len=80) :: name
! Data to be read from netcdf file
  real, allocatable :: arr(:,:)

!  nargs = command_argument_count ()
!  do n=1,nargs
!    call get_command_argument (n, arg)
!  end do

  call mpi_init (ierr)
  call mpi_comm_size (MPI_COMM_WORLD, nranks, ierr)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ierr)
  procname = ' '
  call mpi_get_processor_name (procname, resultlen, ierr)
  write(6,*)'rank=',iam,' is running on node=',trim(procname)

  allocate (nipn(0:nranks-1))
  allocate (begipn(0:nranks-1))
  allocate (endipn(0:nranks-1))

  ret = gptlinitialize ()
  ret = gptlstart ('total')

! All read tasks need to open the input netcdf file
  call handle_err (nf90_open (path='test.nc', mode=NF90_NOWRITE, ncid=ncid))

  call handle_err (nf90_inq_dimid (ncid, 'nip', nip_dimid))
  call handle_err (nf90_inquire_dimension (ncid, nip_dimid, name, nip))

  call handle_err (nf90_inq_dimid (ncid, 'nz', nz_dimid))
  call handle_err (nf90_inquire_dimension (ncid, nz_dimid, name, nz))

  call handle_err (nf90_inq_dimid (ncid, 'time', time_dimid))
  call handle_err (nf90_inquire_dimension (ncid, time_dimid, name, ntime))

  call handle_err (nf90_inq_varid (ncid, 'var', varid))
  call handle_err (nf90_inquire_variable (ncid, varid, name, dimids=dimids))

  if (dimids(1) /= nz_dimid) then
    write(6,*)'Expected dimids(1)=nip_dimid=',nip_dimid,' but got ',dimids(1)
    stop 999
  end if
  if (dimids(2) /= nip_dimid) then
    write(6,*)'Expected dimids(2)=nz_dimid=',nz_dimid,' but got ',dimids(2)
    stop 999
  end if
  if (dimids(3) /= time_dimid) then
    write(6,*)'Expected dimids(3)=time_dimid=',time_dimid,' but got ',dimids(3)
    stop 999
  end if

! Set up the ipn ownership array
  numleft = nip
  begipn(0) = 1
  do n=0,nranks-1
    ranksleft = nranks - n
    if (mod(numleft, ranksleft) == 0) then
      nipn(n) = numleft/ranksleft
    else
      nipn(n) = numleft/ranksleft + 1
    end if
    endipn(n) = begipn(n) + nipn(n) - 1
    numleft = numleft - nipn(n)
    if (n < nranks-1) then
      begipn(n+1) = endipn(n) + 1
    end if
  end do

  allocate (arr(nz,begipn(iam):endipn(iam)))
  arr(:,:) = -1.

! Every task reads the data for the ipn range he owns

  do t=1,ntime
    start(1) = 1
    kount(1) = nz
    start(2) = begipn(iam)
    kount(2) = nipn(iam)
    start(3) = t
    kount(3) = 1
    write(6,*)'iam=',iam,' t=',t,' reading nz=',nz,' nipn=',nipn(iam),' values'
    ret = gptlstart ('get_var')
    call handle_err (nf90_get_var (ncid, varid, arr, start=start, count=kount))
    ret = gptlstop ('get_var')

! Verify contents of data just read in
    do ipn=begipn(iam),endipn(iam)
      do k=1,nz
        if (arr(k,ipn) /= ipn + k + t) then
          write(6,*)'iam=',iam,' k,ipn,t=',k,ipn,t,' arr=',arr(k,ipn),' expected ',ipn + k + t
          call mpi_abort (MPI_COMM_WORLD, 999, ierr)
        end if
      end do
    end do
    call mpi_barrier (MPI_COMM_WORLD, ierr)
  end do
  ret = gptlstop ('total')
  ret = gptlpr_summary (MPI_COMM_WORLD)

  call handle_err (nf90_close (ncid))
  call mpi_finalize (ierr)
  stop
end program readnc
