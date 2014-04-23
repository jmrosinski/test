subroutine xpose_chunks (arrin, arrout, m, n, mm, nchunks, niter)
  implicit none

  integer, intent(in) :: m, n, mm, nchunks, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout(n,chunksize,nchunks,niter)

  integer :: i, ii, j, iter

!$OMP PARALLEL DO PRIVATE(iter,c,i,ii,j)
!ACC$REGION(<chunksize>,<nchunks>,<arrin,arrout:none>) BEGIN
  do iter=1,niter
!ACC$DO PARALLEL (1)
    do c=1,nchunks
!ACC$DO VECTOR (1)
      do ii=1,chunksize
        i = (c-1)*chunksize + (ii-1)
        if (i <= m) then
          do j=1,n
            arrout(j,ii,c,iter) = arrin(i,j,iter)
          end do
        end if
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_chunks
