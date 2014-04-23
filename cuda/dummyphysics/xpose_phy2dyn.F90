subroutine xpose_phy2dyn (nz, ims, ime, physout, dynout, nchunks, chunksize, nvars_phy2dyn)
  implicit none

  integer, intent(in) :: nz, ims, ime, nchunks, chunksize, nvars_phy2dyn
  real, intent(out) :: physout(chunksize,nchunks,nz,nvars_phy2dyn)
  real, intent(in) :: dynout(nz,ims:ime,nvars_phy2dyn)

  integer :: i, ii, c, n, k

!ACC$REGION(<chunksize>,<nz>,<physout:in>,<dynout:out>) BEGIN
  do n=1,nvars_phy2dyn
!ACC$DO PARALLEL (1)
    do k=1,nz
      do c=1,nchunks
!ACC$DO VECTOR (1)
        do i=1,chunksize
          ii = min (ime, ims + (c-1)*chunksize + (i-1))
          dynout(k,ii,n) = physout(i,c,k,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_phy2dyn
