subroutine xpose_direct (arrin, arrout, m, n, niter)
  implicit none

  integer, intent(in) :: m, n, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout(n,m,niter)

  integer :: i, j, iter

!ACC$REGION(<n>,<m>,<arrin,arrout:none>) BEGIN
!$OMP PARALLEL DO PRIVATE(iter,i,j)
  do iter=1,niter
!ACC$DO PARALLEL (1)
    do i=1,m
!ACC$DO VECTOR (1)
      do j=1,n
        arrout(j,i,iter) = arrin(i,j,iter)
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_direct
