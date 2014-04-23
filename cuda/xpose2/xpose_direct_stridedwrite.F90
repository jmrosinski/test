subroutine xpose_direct_stridedwrite (arrin, arrout, m, n, niter)
  implicit none

  integer, intent(in) :: m, n, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout(n,m,niter)

  integer :: i, j, iter

!ACC$REGION(<m>,<n>,<arrin,arrout:none>) BEGIN
!$OMP PARALLEL DO PRIVATE(iter,i,j)
  do iter=1,niter
!ACC$DO PARALLEL (1)
    do j=1,n
!ACC$DO VECTOR (1)
      do i=1,m
        arrout(j,i,iter) = arrin(i,j,iter)
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_direct_stridedwrite
