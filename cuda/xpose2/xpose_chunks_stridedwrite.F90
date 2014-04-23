subroutine xpose_chunks_stridedwrite (arrin, arrout2, m, n, chunksize, nchunks, niter)
  implicit none

  integer, intent(in) :: m, n, chunksize, nchunks, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout2(n,chunksize,nchunks,niter)

  integer :: i, c, ii, j, iter

!$OMP PARALLEL DO PRIVATE(iter,c,ii,i,j)
!ACC$REGION(<chunksize>,<nchunks>,<arrin,arrout2:none>) BEGIN
  do iter=1,niter
!ACC$DO PARALLEL (1)
    do c=1,nchunks
      do j=1,n
!ACC$DO VECTOR (1)
        do ii=1,chunksize
          i = (c-1)*chunksize + ii
          if (i <= m) then
            arrout2(j,ii,c,iter) = arrin(i,j,iter)
          end if
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_chunks_stridedwrite
