program main
  use netcdf

  implicit none

  integer, parameter :: nx = 128
  integer, parameter :: ny = 64
  integer, parameter :: nz = 9
  integer, parameter :: nt = 2   ! number of time periods

  integer :: ret           ! return from MPI call
  integer :: ncid          ! netcdf file id
  integer :: mode_flag     ! creation flag for netcdf
  integer :: dimids(4)     ! dimension ids for a variable
! dimension ids
  integer :: lon_dimid, lat_dimid, lev_dimid, time_dimid
! variable ids
  integer :: lonid, latid, levid, timeid ! dimension variable ids
  integer :: uid, uid_compressed
  integer :: startlat, endlat    ! starting, ending latitudes for the MPI task
  integer :: numlats             ! number of lats this MPI task responsible for
  integer :: start(4)            ! start array for nf90_put_var
  integer :: count(3)            ! count array for nf90_put_var
  integer :: i, j, k, n          ! loop indices over lon, lat, lev, time

! dimension variables
  real :: lon(nx)
  real :: lat(ny)
  real :: lev(nz)

! model variables
  real :: val(1)
  real, allocatable :: u(:,:,:) ! array holds data to be written

!  mode_flag = ior (nf90_noclobber, nf90_hdf5)
  mode_flag = ior (nf90_clobber, nf90_netcdf4)
! See http://www.unidata.ucar.edu/support/help/MailArchives/netcdf/msg08863.html
! This only seems to work on my workstation (netcdf 4.1.3)

! Create the netcdf file. After creation, it will be open in define mode
  call handle_err (nf90_create ("test.nc", mode_flag, ncid))
! Define dimensions
  call handle_err (nf90_def_dim (ncid, "lon", nx, lon_dimid))
  call handle_err (nf90_def_dim (ncid, "lat", ny, lat_dimid))
  call handle_err (nf90_def_dim (ncid, "lev", nz, lev_dimid))
! Time is an unlimited dimension
  call handle_err (nf90_def_dim (ncid, "time", nf90_unlimited, time_dimid))

! Define dimension variables and attributes
  call handle_err (nf90_def_var (ncid, "lon", nf90_float, lon_dimid, lonid))
  call handle_err (nf90_put_att (ncid, lonid, "standard_name", "longitude"))
  call handle_err (nf90_put_att (ncid, lonid, "long_name", "longitude"))
  call handle_err (nf90_put_att (ncid, lonid, "units", "degrees_east"))

  call handle_err (nf90_def_var (ncid, "lat", nf90_float, lat_dimid, latid))
  call handle_err (nf90_put_att (ncid, latid, "standard_name", "latitude"))
  call handle_err (nf90_put_att (ncid, latid, "long_name", "latitude"))
  call handle_err (nf90_put_att (ncid, latid, "units", "degrees_north"))

  call handle_err (nf90_def_var (ncid, "lev", nf90_float, lev_dimid, levid))
  call handle_err (nf90_put_att (ncid, levid, "standard_name", "pressure"))
  call handle_err (nf90_put_att (ncid, levid, "long_name", "pressure"))
  call handle_err (nf90_put_att (ncid, levid, "units", "Pa"))

  call handle_err (nf90_def_var (ncid, "time", nf90_float, time_dimid, timeid))
  call handle_err (nf90_put_att (ncid, timeid, "standard_name", "time"))
  call handle_err (nf90_put_att (ncid, timeid, "units", "days since 1990-1-1 0:0:0"))

! Define 3-d variables: 1 uncompressed and 1 compressed (to level 9 which means lots of compression)
  dimids(1) = lon_dimid
  dimids(2) = lat_dimid
  dimids(3) = lev_dimid
  dimids(4) = time_dimid
  call handle_err (nf90_def_var (ncid, "u_wind", nf90_float, dimids, uid))
!  call handle_err (nf90_def_var (ncid, "u_wind_compressed", nf90_float, dimids, uid_compressed, deflate_level=1))
! End define mode and enter data mode
  call handle_err (nf90_enddef (ncid))

! Define and write dimension variables
  do i=1,nx
    lon(i) = (i-1)/360.
  end do

  do j=1,ny
    lat(j) = (j-1-ny/2)/180.
  end do

  do k=1,nz
    lev(k) = (k*1.013e5)/nz
  end do

! Master task writes the dimension variables
  call handle_err (nf90_put_var (ncid, lonid, lon))
  call handle_err (nf90_put_var (ncid, latid, lat))
  call handle_err (nf90_put_var (ncid, levid, lev))
    
! Determine my portion of the data to write
  startlat = 1
  endlat = ny
  numlats = endlat - startlat + 1
  write(6,*) ' startlat=', startlat, ' endlat=', endlat

  allocate (u(nx,startlat:endlat,nz))

! Time loop
  do n=1,nt
    do k=1,nz
      do j=startlat,endlat
        do i=1,nx
          u(i,j,k) = real(n)/(i+j+k)
        end do
      end do
    end do

! Write time dimension var. Start at 0 days since ...
    start(1) = n
    count(1) = 1
    val(1) = float(n-1)
    call handle_err (nf90_put_var (ncid, timeid, val, start=start, count=count))

    start = (/1 ,startlat,1 ,n/)
    count = (/nx,numlats ,nz/)

    call handle_err (nf90_put_var (ncid, uid, u, start=start, count=count))
!    call handle_err (nf90_put_var (ncid, uid_compressed, u, start=start, count=count))
  end do

  call handle_err (nf90_close (ncid))
  stop 0
end program main

subroutine handle_err (code)
  use netcdf

  implicit none

  integer, intent(in) :: code ! netcdf error code
  integer :: ret

  if (code /= nf90_noerr) then
    write(6,*) trim (nf90_strerror (code))
    stop 999
  end if
end subroutine handle_err
