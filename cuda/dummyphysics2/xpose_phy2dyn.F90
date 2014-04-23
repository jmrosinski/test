subroutine xpose_phy2dyn (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn, physout, dynout)
  implicit none

  integer, intent(in) :: nz, ims, ime, nchunks, chunksize, nvars_phy2dyn
  real, intent(out) :: physout(ims:ime,nz,nvars_phy2dyn)
  real, intent(in) :: dynout(nz,ims:ime,nvars_phy2dyn)

  integer :: i, c, n, k, ipn

!ACC$REGION(<chunksize>,<nchunks>,<physout:in>,<dynout:out>) BEGIN
  do n=1,nvars_phy2dyn
    do k=1,nz
!ACC$DO PARALLEL (1)
      ipn = 0        ! For some reason F2C-ACC complains if this isnt initialized
      do c=1,nchunks
!ACC$DO VECTOR (1)
        do i=1,chunksize
          ipn = min (ipe, (ips + (c-1)*chunksize) + (i-1))
          dynout(k,ipn,n) = physout(ipn,k,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_phy2dyn
