subroutine xpose_phy2dyn (nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn, physout, dynout)
  implicit none

  integer, intent(in) :: nz, ims, ime, ips, ipe, nchunks, chunksize, nvars_phy2dyn
  real, intent(in) :: physout(chunksize,nchunks,nz,nvars_phy2dyn)
  real, intent(out) :: dynout(nz,ims:ime,nvars_phy2dyn)

  integer :: i, c, n, k, ipn, ipnstart

#ifdef GPU
!ACC$REGION(<nz>,<ipe-ips+1>,<physout,dynout:none>) BEGIN
  do n=1,nvars_phy2dyn
! Strided read
!ACC$DO PARALLEL (1)
    do ipn=ips,ipe
      c = (ipn-ips)/chunksize + 1
      i = (ipn-ips) - (c-1)*chunksize + 1
!ACC$DO VECTOR (1)
      do k=1,nz
        dynout(k,ipn,n) = physout(i,c,k,n)
      end do
    end do
  end do
!ACC$REGION END

#else

!$OMP PARALLEL DO PRIVATE(c,n,i,ipn,k)
  do c=1,nchunks
    do n=1,nvars_phy2dyn
      do i=1,chunksize
        ipn = ips + (c-1)*chunksize + (i-1)
        if (ipn <= ipe) then
          do k=1,nz
            dynout(k,ipn,n) = physout(i,c,k,n)
          end do
        end if
      end do
    end do
  end do

#endif

  return
end subroutine xpose_phy2dyn
