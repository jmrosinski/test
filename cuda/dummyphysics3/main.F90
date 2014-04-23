program main
  use gptl
! Test program transposes an array on the GPU and sees that it's correct
  implicit none

!  integer, parameter :: nip = 10
!  integer, parameter :: maxtpb = 8 ! max threads per block (see output from deviceQuery)
!  integer :: nz = 2

  integer, parameter :: nip = 10242
  integer :: nz = 32
  integer :: nvars_dyn2phy=200
  integer :: nvars_phy2dyn=200

  integer :: ims=1
  integer :: ime=nip
  integer :: its=1

  integer :: ips=1
  integer :: ipe=nip
  integer :: ipn, k, n
  integer :: i, c
  integer :: ret

  integer :: chunksize, nchunks
  integer, external :: omp_get_max_threads

  real*8 :: val
  real :: rate
  real, allocatable :: dynvars(:,:,:)
  real, allocatable :: physvars(:,:,:,:)
  real, allocatable :: physout(:,:,:,:)
  real, allocatable :: dynout(:,:,:)

  chunksize = 128
  if (mod (nip,chunksize) == 0) then
    nchunks = nip/chunksize
  else
    nchunks = nip/chunksize + 1
  end if

  allocate (dynvars(nz,ims:ime,nvars_dyn2phy))
  allocate (physvars(chunksize,nchunks,nz,nvars_dyn2phy))
  allocate (physout(chunksize,nchunks,nz,nvars_phy2dyn))
  allocate (dynout(nz,ims:ime,nvars_phy2dyn))

  dynout(:,:,:) = -999.

  do n=1,nvars_dyn2phy
! Create dummy data for dynvars
    do ipn=1,nip
      do k=1,nz
        dynvars(k,ipn,n) = 1./(k+ipn+n)
      end do
    end do
  end do

#ifdef GPU
  write(6,*)'GPU code'
  call copyin (dynvars, nz, ims, ime, chunksize, nchunks, nvars_dyn2phy, nvars_phy2dyn)
#else
  write(6,*)'CPU code: num threads=', omp_get_max_threads()
#endif
  write(6,*)'chunksize=', chunksize

  ret = gptlsetutr (gptlnanotime)
  ret = gptlinitialize ()
  write(6,*)'calling xpose_dyn2phy'
  ret = gptlstart ('xpose_dyn2phy')
  call xpose_dyn2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy, dynvars, physvars)
  ret = gptlstop ('xpose_dyn2phy')

  write(6,*)'calling physics'
  call physics (nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn, &
                physvars, physout)

  write(6,*)'calling xpose_phy2dyn'
  ret = gptlstart ('xpose_phy2dyn')
  call xpose_phy2dyn (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn, physout, dynout)
  ret = gptlstop ('xpose_phy2dyn')

#ifdef GPU
  call copyout (dynout, nz, ims, ime, chunksize, nchunks, nvars_phy2dyn)
#endif

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

  ret = gptlget_wallclock ('xpose_dyn2phy', 0, val)
  rate = nvars_dyn2phy * nip * nz * 2. * 4. / (1.e6 * val)
  write(6,*)'xpose_dyn2phy BW = ', rate, ' MB/s'

  ret = gptlget_wallclock ('xpose_phy2dyn', 0, val)
  rate = nvars_phy2dyn * nip * nz * 2. * 4. / (1.e6 * val)
  write(6,*)'xpose_phy2dyn BW = ', rate, ' MB/s'

  write(6,*)'calling copy_dyn2dyn'
  ret = gptlstart ('copy_dyn2dyn')
  call copy_phy2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn, physvars, physout)
  ret = gptlstop ('copy_dyn2dyn')

  ret = gptlget_wallclock ('copy_dyn2dyn', 0, val)
  rate = nvars_phy2dyn * nip * nz * 2. * 4. / (1.e6 * val)
  write(6,*)'copy_dyn2dyn BW = ', rate, ' MB/s'

  ret = gptlprint_memusage ('End of run')
  ret = gptlpr (0)

  stop 0
end program main
