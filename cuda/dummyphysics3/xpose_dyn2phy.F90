subroutine xpose_dyn2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy, dynvars, physvars)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy
  real, intent(in) :: dynvars(nz,ims:ime,nvars_dyn2phy)
  real, intent(out) :: physvars(chunksize,nchunks,nz,nvars_dyn2phy)

  integer :: i, c, n, k, ipn

#ifdef GPU

!ACC$REGION(<nz>,<nchunks>,<dynvars,physvars:none>) BEGIN
  do n=1,nvars_dyn2phy
!ACC$DO PARALLEL (1)
    do c=1,nchunks
      do i=1,chunksize
        ipn = ips + (c-1)*chunksize + (i-1)
        if (ipn <= ipe) then
!ACC$DO VECTOR (1)
          do k=1,nz
            physvars(i,c,k,n) = dynvars(k,ipn,n)
          end do
        end if
      end do
    end do
  end do
!ACC$REGION END

#else

!$OMP PARALLEL DO PRIVATE(c,n,k,i,ipn)
  do c=1,nchunks
    do n=1,nvars_dyn2phy
      do i=1,chunksize
        ipn = ips + (c-1)*chunksize + (i-1)
        if (ipn <= ipe) then
          do k=1,nz
            physvars(i,c,k,n) = dynvars(k,ipn,n)
          end do
        end if
      end do
    end do
  end do

#endif

  return
end subroutine xpose_dyn2phy
