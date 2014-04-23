subroutine physics (nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn, &
                    dt, cp, physvars, physout, he3dik, rik, eik)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn
  real, intent(in) :: dt, cp
  real, intent(in)    :: physvars(ims:ime,nz,nvars_dyn2phy) ! u, v, t, etc. from dyn
  real, intent(out)   :: physout (ims:ime,nz,nvars_phy2dyn)  ! external heating function
  real, intent(in)    :: he3dik(ims:ime,nz)
  real, intent(in)    :: rik(ims:ime,nz)
  real, intent(in)    :: eik(ims:ime,nz)


  integer :: i, k, n, c, ipn
  integer :: nchunks
  integer :: npts 
  integer :: nvars


!ACC$REGION(<chunksize>,<nchunks>,<physvars,he3dik,rik,eik:in>,<physout:out>) BEGIN
  real, parameter :: rad      = 1.e6
  real, parameter :: hamp     = 0.2
  real, parameter :: htime    = 0.5
  real, parameter :: hrise    = 0.5
  real, parameter :: ttime    = 3600.*htime
  real, parameter :: trise    = 3600.*hrise
  real            :: hf,ts,time

  real :: rcp

  rcp = 1./cp
  nvars = min (nvars_dyn2phy, nvars_phy2dyn)
  do n=1,nvars
    do k=1,nz
!ACC$DO PARALLEL (1)
      ipn = 0
      do c=1,nchunks
!ACC$DO VECTOR (1)
        do i=1,chunksize
          ipn = min (ipe, (ips + (c-1)*chunksize + (i-1)))
          physout(ipn,k,1) = hf*hamp*rik(ipn,k)*he3dik(ipn,k)/(cp*eik(ipn,k))
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine physics
