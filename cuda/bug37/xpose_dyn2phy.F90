subroutine xpose_dyn2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, &
                          nvars_dyn2phy, dynvars, physvars)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy
  real, intent(in) :: dynvars(nz,ims:ime,nvars_dyn2phy)
  real, intent(out) :: physvars(chunksize,nchunks,nz,nvars_dyn2phy)

  integer :: i, c, n, k, ipn

!ACC$REGION(<chunksize>,<nchunks>,<dynvars:in>,<physvars:out>) BEGIN
  do n=1,nvars_dyn2phy
!ACC$DO PARALLEL (1)
    do c=1,nchunks
      do k=1,nz
!ACC$DO VECTOR (1)
        do i=1,chunksize
          ipn = min (ipe, (ips + (c-1)*chunksize + (i-1)))
          physvars(i,c,k,n) = dynvars(k,ipn,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_dyn2phy
