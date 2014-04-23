subroutine copyout (arrout, m, n, niter)
  implicit none

  integer, intent(in) :: m, n, niter
  real, intent(inout) :: arrout(n,m,niter)

!ACC$DATA(<arrout:out>)

  return
end subroutine copyout
