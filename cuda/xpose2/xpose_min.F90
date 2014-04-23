subroutine xpose_min (arrin, arrout, m, n, mm, nn, niter)
  implicit none

  integer, intent(in) :: m, n, mm, nn, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout(n,m,niter)

  real :: subarr(nn,mm)

  integer :: i, ii, j, jj, iter
  integer :: iiters, jiters

!ACC$DATA(<subarr:none,shared>)

!ACC$REGION(<nn>,<n>,<arrin,arrout:none>) BEGIN
!$OMP PARALLEL DO PRIVATE(iter,j,i,jj,ii,subarr,iiters,jiters)
  do iter=1,niter
    jiters = 0
!ACC$DO PARALLEL (1)
    do j=1,n,nn
      iiters = 0
      do i=1,m,mm
!ACC$DO VECTOR (1)
        do jj=1,min(nn,n-jiters*nn)
          do ii=1,min(mm,m-iiters*mm)
            subarr(jj,ii) = arrin(i+ii-1,j+jj-1,iter)
          end do
        end do
        
        do ii=1,min(mm,m-iiters*mm)
!ACC$DO VECTOR (1)
          do jj=1,min(nn,n-jiters*nn)
            arrout(j+jj-1,i+ii-1,iter) = subarr(jj,ii)
          end do
        end do
        iiters = iiters + 1
      end do
      jiters = jiters + 1
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_min
