subroutine xpose_dyn2phy(nz, ims, ime, dynvars, physvars, nchunks, chunksize, nvars_dyn2phy)
  implicit none

  integer, intent(in) :: nz, ims, ime, nchunks, chunksize, nvars_dyn2phy
  real, intent(in) :: dynvars(nz,ims:ime,nvars_dyn2phy)
  real, intent(out) :: physvars(chunksize,nchunks,nz,nvars_dyn2phy)

  integer :: i, c, n, k, ipn

!ACC$REGION(<nz>,<nchunks>,<dynvars:in>,<physvars:out>) BEGIN
  do n=1,nvars_dyn2phy
!ACC$DO PARALLEL (1)
    ipn = ims - 1
    do c=1,nchunks
      do i=1,chunksize
        ipn = min (ime,ipn+1)
!ACC$DO VECTOR (1)
        do k=1,nz
          physvars(i,c,k,n) = dynvars(k,ipn,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_dyn2phy
