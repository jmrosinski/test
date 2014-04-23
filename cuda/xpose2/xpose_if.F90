subroutine xpose_if (arrin, arrout, m, n, mm, nn, niter)
  implicit none

  integer, intent(in) :: m, n, mm, nn, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(out) :: arrout(n,m,niter)

  real :: subarr(mm,nn)

  integer :: i, ii, j, jj, iter

!ACC$DATA(<subarr:none,shared>)

!ACC$REGION(<mm>,<nn>,<arrin,arrout:none>) BEGIN
!$OMP PARALLEL DO PRIVATE(iter,j,i,jj,ii,subarr)
  do iter=1,niter
    do j=1,n,nn
      do i=1,m,mm
!ACC$DO PARALLEL (1)
        do jj=1,nn
          if (j+jj-1 <= n) then
!ACC$DO VECTOR (1)
            do ii=1,mm
              if (i+ii-1 <= m) then
                subarr(ii,jj) = arrin(i+ii-1,j+jj-1,iter)
              end if
            end do
          end if
        end do

!ACC$DO PARALLEL (1)
        do jj=1,nn
          if (j+jj-1 <= n) then
!ACC$DO VECTOR (1)
            do ii=1,mm
              if (i+ii-1 <= m) then
                arrout(j+jj-1,i+ii-1,iter) = subarr(ii,jj)
              end if
            end do
          end if
        end do
      end do
    end do
  end do
!ACC$REGION END

  return
end subroutine xpose_if
