subroutine copyin (dynvars, nz, ims, ime, chunksize, nchunks, nvars_dyn2phy, nvars_phy2dyn)
  implicit none

  integer, intent(in) :: nz, ims, ime, chunksize, nchunks, nvars_dyn2phy, nvars_phy2dyn
  real, intent(in) :: dynvars(nz,ims:ime,nvars_dyn2phy)
  real :: physvars(chunksize,nchunks,nz,nvars_dyn2phy)
  real :: physout(chunksize,nchunks,nz,nvars_phy2dyn)
  real :: dynout(nz,ims:ime,nvars_phy2dyn)

!ACC$DATA(<dynvars:in>,<physvars,physout,dynout:none>)

  return
end subroutine copyin
