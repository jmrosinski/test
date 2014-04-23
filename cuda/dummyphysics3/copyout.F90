subroutine copyout (dynout, nz, ims, ime, chunksize, nchunks, nvars_phy2dyn)
  implicit none

  integer, intent(in) :: nz, ims, ime, chunksize, nchunks, nvars_phy2dyn
  real, intent(inout) :: dynout(nz,ims:ime,nvars_phy2dyn)

!ACC$DATA(<dynout:out>)

  return
end subroutine copyout
