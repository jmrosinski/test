program writenc
  use netcdf
  use gptl
  implicit none

  integer, parameter :: glvl = 9
  integer, parameter :: nip = 2621442
  integer, parameter :: nz = 96
  integer, parameter :: nt = 10          ! number of time levels
  integer :: nargs
  integer :: k, n, t
  integer :: ipn
  integer :: ret             ! gptl return
! Netcdf ids
  integer :: ncid  ! file id
  integer :: nip_dimid, nipid
  integer :: nz_dimid, nzid
  integer :: time_dimid, timeid
  integer :: varid
  integer :: dimids(3)  ! enough dims to hold nip, nz, time
  integer :: start(3), kount(3)
! Data to be written to netcdf file
  real :: arr(nz,nip)
  real :: ipnarr(nip)
  real :: nzarr(nz)

!  nargs = command_argument_count ()
!  do n=1,nargs
!    call get_command_argument (n, arg)
!  end do

! Open the output netcdf file
! G8 won't run without NF90_64BIT_OFFSET
  call handle_err (nf90_create (path='test.nc', &
       cmode=or(NF90_CLOBBER, NF90_64BIT_OFFSET), & 
       ncid=ncid))
  call handle_err (nf90_def_dim (ncid, 'nz', nz, nz_dimid))
  call handle_err (nf90_def_dim (ncid, 'nip', nip, nip_dimid))
  call handle_err (nf90_def_dim (ncid, 'time', NF90_UNLIMITED, time_dimid))

  call handle_err (nf90_def_var (ncid, 'nz',  NF90_INT, nz_dimid, nzid))
  call handle_err (nf90_def_var (ncid, 'nip',  NF90_INT, nip_dimid, nipid))
  call handle_err (nf90_def_var (ncid, 'time',  NF90_INT, time_dimid, timeid))

  dimids(1) = nzid
  dimids(2) = nipid
  dimids(3) = timeid
  call handle_err (nf90_def_var (ncid, 'var',  NF90_REAL, dimids(1:3), varid))

! Done defining metada. End define mode so we can now write data
  call handle_err (nf90_enddef (ncid))
  ret = gptlinitialize ()
  ret = gptlstart ('total')

  do ipn=1,nip
    ipnarr(ipn) = ipn
  end do
  start(1) = 1
  kount(1) = nip
  call handle_err (nf90_put_var (ncid, nipid, ipnarr, start=start(1:1), count=kount(1:1)))

  do k=1,nz
    nzarr(k) = k
  end do
  start(1) = 1
  kount(1) = nz
  call handle_err (nf90_put_var (ncid, nzid, nzarr, start=start(1:1), count=kount(1:1)))

  do t=1,nt
    do k=1,nz
      do ipn=1,nip
        arr(k,ipn) = ipn + k + t
      end do
    end do

    write(6,*) 'Writing data for t=',t
    do ipn=1,nip
      start(1) = 1
      kount(1) = nz
      start(2) = ipn
      kount(2) = 1
      start(3) = t
      kount(3) = 1
      ret = gptlstart ('put_var')
      call handle_err (nf90_put_var (ncid, varid, arr(:,ipn), start=start, count=kount))
      ret = gptlstop ('put_var')
    end do
  end do

  call handle_err (nf90_close (ncid))
  ret = gptlpr (0)
  stop
end program writenc
