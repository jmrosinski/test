subroutine physics (nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn, &
                    physvars, physout)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, chunksize, nchunks, its, nvars_dyn2phy, nvars_phy2dyn
  real, intent(in)    :: physvars(chunksize,nchunks,nz,nvars_dyn2phy) ! u, v, t, etc. from dyn
  real, intent(out)   :: physout (chunksize,nchunks,nz,nvars_phy2dyn)  ! external heating function

  integer :: i, k, n, c
  integer :: npts 
  integer :: nvars

!ACC$REGION(<chunksize>,<nchunks>,<physvars,physout:none>) BEGIN
  nvars = min (nvars_dyn2phy, nvars_phy2dyn)
  do n=1,nvars
    do k=1,nz
!ACC$DO PARALLEL (1)
      do c=1,nchunks
!ACC$DO VECTOR (1)
        do i=1,chunksize
          physout(i,c,k,n) = physvars(i,c,k,n)
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine physics
