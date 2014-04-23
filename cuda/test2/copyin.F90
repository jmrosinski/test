subroutine copyin (arrin, m, n, niter)
  implicit none

  integer, intent(in) :: m, n, niter
  real, intent(in) :: arrin(m,n,niter)
  real :: arrout(n,m,niter)

!ACC$DATA(<arrin:in>,<arrout:none>)

  return
end subroutine copyin
