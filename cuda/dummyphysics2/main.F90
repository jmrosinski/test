program main
! Test program transposes an array on the GPU and sees that it's correct
  implicit none

!  integer, parameter :: nip = 10
!  integer, parameter :: maxtpb = 8 ! max threads per block (see output from deviceQuery)
!  integer :: nz = 2

  integer, parameter :: nip = 10242
  integer, parameter :: maxtpb = 128 ! max threads per block (see output from deviceQuery)
  integer :: nz = 32
  integer :: nvars_dyn2phy=2
  integer :: nvars_phy2dyn=2

  integer :: ims=1
  integer :: ime=nip
  integer :: its=1

  integer :: ips=1
  integer :: ipe=nip
  integer :: ipn, k, n
  integer :: i, c

  integer :: chunksize, nchunks

  real :: dt = 4.
  real :: cp = 1004.

  real, allocatable :: dynvars(:,:,:)
  real, allocatable :: physvars(:,:,:)
  real, allocatable :: physout(:,:,:)
  real, allocatable :: dynout(:,:,:)

  real, allocatable :: he3dik(:,:)
  real, allocatable :: rik(:,:)
  real, allocatable :: eik(:,:)

  chunksize = min (nip,maxtpb)
  if (mod (nip,chunksize) == 0) then
    nchunks = nip/chunksize
  else
    nchunks = nip/chunksize + 1
  end if

  allocate (dynvars(nz,ims:ime,nvars_dyn2phy))
  allocate (physvars(ims:ime,nz,nvars_dyn2phy))
  allocate (physout(ims:ime,nz,nvars_phy2dyn))
  allocate (dynout(nz,ims:ime,nvars_phy2dyn))

  allocate (he3dik(ims:ime,nz))
  allocate (rik(ims:ime,nz))
  allocate (eik(ims:ime,nz))

  he3dik(:,:) = 1.
  rik(:,:) = 2.
  eik(:,:) = 3.

  dynout(:,:,:) = -999.

  do n=1,nvars_dyn2phy
! Create dummy data for dynvars
    do ipn=1,nip
      do k=1,nz
        dynvars(k,ipn,n) = 1./(k+ipn+n)
      end do
    end do
  end do

  write(6,*)'calling xpose_dyn2phy'
  call xpose_dyn2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy, dynvars, physvars)

  write(6,*)'calling physics'
  call physics (nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn, &
                dt, cp, physvars, physout, he3dik, rik, eik)

  write(6,*)'calling xpose_phy2dyn'
  call xpose_phy2dyn (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn, physout, dynout)

! Verify correctness of results
  do n=1,min(nvars_dyn2phy,nvars_phy2dyn)
    do ipn=1,nip
      do k=1,nz
        if (dynout(k,ipn,n) /= dynvars(k,ipn,n)) then
          write(6,*) 'k,ipn,n,dynvars,dynout=',k,ipn,n,dynvars(k,ipn,n),dynout(k,ipn,n)
          stop 999
        end if
      end do
    end do
  end do

  write(6,*)'physics did the right thing!'  
  stop 0
end program main
