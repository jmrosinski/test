program main
! Test program transposes an array on the GPU and sees that it's correct
  implicit none

!  integer, parameter :: nip = 10
!  integer, parameter :: maxtpb = 8 ! max threads per block (see output from deviceQuery)
!  integer :: nz = 2

  integer, parameter :: nip = 10242
  integer, parameter :: maxtpb = 1024 ! max threads per block (see output from deviceQuery)
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

  real, allocatable :: dynvars(:,:,:)
  real, allocatable :: physvars(:,:,:,:)
  real, allocatable :: physout(:,:,:,:)
  real, allocatable :: dynout(:,:,:)

  chunksize = min (nip,maxtpb)
  if (mod (nip,chunksize) == 0) then
    nchunks = nip/chunksize
  else
    nchunks = nip/chunksize + 1
  end if

  allocate (dynvars(nz,nip,nvars_dyn2phy))
  allocate (physvars(chunksize,nchunks,nz,nvars_dyn2phy))
  allocate (physout(chunksize,nchunks,nz,nvars_phy2dyn))
  allocate (dynout(nz,nip,nvars_phy2dyn))

  dynout(:,:,:) = -999.

  do n=1,nvars_dyn2phy
! Create dummy data for dynvars
    do ipn=1,nip
      do k=1,nz
        dynvars(k,ipn,n) = 1./(k+ipn+n)
      end do
    end do
  end do

  call xpose_dyn2phy (nz, ims, ime, dynvars, physvars, nchunks, chunksize, nvars_dyn2phy)

  call physics (nz, chunksize, nchunks, its, physvars, physout, &
                nvars_dyn2phy, nvars_phy2dyn)

  call xpose_phy2dyn (nz, ims, ime, physout, dynout, nchunks, chunksize, nvars_phy2dyn)

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
