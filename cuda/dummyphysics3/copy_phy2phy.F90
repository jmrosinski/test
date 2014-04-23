subroutine copy_phy2phy (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy, physvars, physout)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_dyn2phy
  real, intent(in) :: physvars(chunksize,nchunks,nz,nvars_dyn2phy)
  real, intent(out) :: physout(chunksize,nchunks,nz,nvars_dyn2phy)

  integer :: i, c, n, k, ipn

#ifdef GPU

!ACC$REGION(<chunksize>,<nchunks>,<physvars,physout:none>) BEGIN
  do n=1,nvars_dyn2phy
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

#else

!$OMP PARALLEL DO PRIVATE(c,n,i,k)
  do c=1,nchunks
    do n=1,nvars_dyn2phy
      do k=1,nz
        do i=1,chunksize
          physout(i,c,k,n) = physvars(i,c,k,n)
        end do
      end do
    end do
  end do

#endif

  return
end subroutine copy_phy2phy
