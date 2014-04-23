subroutine driver
  use decomp

!SMS$IGNORE BEGIN
  use mpi

  type phystype
    real*8, pointer :: alvsfout(:)=>null()
    real*8, pointer :: alvsfin(:)=>null()
  end type phystype
!SMS$IGNORE END

  type(phystype) :: physvar
  integer :: ims, ime, ipn
  integer :: nprocs = 1
  integer :: unitno = 1
  integer              :: halosize
  integer,allocatable  :: regionsize(:)
  character(len=80) :: decompinfofile

!SMS$DISTRIBUTE (dh,1) BEGIN
  real*8, allocatable :: arrout(:)
  real*8, allocatable :: arrin(:)
!SMS$DISTRIBUTE END

!SMS$insert call NNT_NPROCS(nprocs)
!SMS$insert allocate(RegionSize(nprocs))
!SMS$insert write(decompinfofile,"('DecompInfo_',i0,'.dat')") nprocs
!SMS$insert open (unitno,file=decompinfofile,status='old',err=71)
!SMS$insert read (unitno,*) HaloSize
!SMS$insert read (unitno,*) RegionSize
!SMS$insert close(unitno)

!SMS$CREATE_DECOMP(dh,<nip>,<halosize>:regionsize=regionsize)

  write(6,*)'halosize=',halosize
  write(6,*)'regionsize=',regionsize

!SMS$PARALLEL(dh,nip) BEGIN
  allocate (arrout(nip))
  allocate (arrin(nip))
!SMS$PARALLEL END

!SMS$IGNORE BEGIN
  ims = lbound(arrout,1)
  ime = ubound(arrout,1)
  allocate (physvar%alvsfout(ims:ime))
  allocate (physvar%alvsfin(ims:ime))
!SMS$IGNORE END

!SMS$PARALLEL(dh,nip) BEGIN
  do ipn=1,nip
    arrout(ipn) = ipn
  end do
!SMS$PARALLEL END

!SMS$IGNORE BEGIN
  do ipn=ims,ime
    physvar%alvsfout(ipn) = ipn
  end do
!SMS$IGNORE END

!SMS$PARALLEL(dh,nip) BEGIN
  unitno = 1
!SMS$SERIAL BEGIN
  open (unit=unitno, file='zzz', form='unformatted', action='write', err=70)
!SMS$SERIAL END
  call writearr64 (arrout, 1, unitno)
  call writearr64 (physvar%alvsfout, 1, unitno)
  write(6,*)'closing unitno'
!SMS$SERIAL BEGIN
  close (unitno)
!SMS$SERIAL END
  write(6,*)'closed unitno'

!SMS$SERIAL BEGIN
  open (unitno, file='zzz', form='unformatted', action='read', err=70)
!SMS$SERIAL END

  call readarr64 (arrin, 1, unitno)
  call readarr64 (physvar%alvsfin, 1, unitno)
!SMS$SERIAL BEGIN
  close (unitno)
!SMS$SERIAL END

  do ipn=1,nip
    if (arrin(ipn) /= arrout(ipn)) then
      write(6,*)'ipn,arrin,arrout=',ipn,arrin(ipn),arrout(ipn)
      stop
    end if

    if (physvar%alvsfin(ipn) /= physvar%alvsfout(ipn)) then
      write(6,*)'ipn,alvsfin,alvsfout=',ipn,physvar%alvsfin(ipn),physvar%alvsfout(ipn)
      stop
    end if
  end do
  
  return
70 write(6,*)'driver: error opening zzz'
  stop
71 write(6,*)'driver: error opening decompinfofile'
  stop
!SMS$PARALLEL END
end subroutine driver
