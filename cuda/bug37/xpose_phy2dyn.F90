subroutine xpose_phy2dyn (nz, ims, ime, ips, ipe, nchunks, chunksize, &
                          nvars_phy2dyn, physout, dynout)
  implicit none

  integer, intent(in) :: nz, ims, ime, nchunks, chunksize, nvars_phy2dyn
  real, intent(out) :: physout(chunksize,nchunks,nz,nvars_phy2dyn)
  real, intent(in) :: dynout(nz,ims:ime,nvars_phy2dyn)

  integer :: i, c, n, k, ipn

!ACC$REGION(<chunksize>,<nchunks>,<physout:in>,<dynout:out>) BEGIN
  do n=1,nvars_phy2dyn
!ACC$DO PARALLEL (1)
    do c=1,nchunks
      do k=1,nz
!ACC$DO VECTOR (1)
        do i=1,chunksize
          ipn = min (ipe, (ips + (c-1)*chunksize + (i-1)))
          dynout(k,ipn,n) = physout(i,c,k,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_phy2dyn
