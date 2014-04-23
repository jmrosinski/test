module globals
  integer, parameter :: prec = 4
end module globals

program main
  use globals
  use gptl

  implicit none

  integer, parameter :: nval = 1000000
  integer, parameter :: niter = 600
  real(prec) :: a(nval) = 0.1
  real(prec) :: b(nval) = 0.1
  real(prec) :: c(nval) = 0.1
  real(prec) :: d(nval) = 0.1
  real(prec) :: e(nval) = 0.1
  real(prec) :: f(nval) = 0.1
  real(prec) :: g(nval) = 0.1
  real(prec) :: h(nval) = 0.1

  integer :: ret
  integer :: code
  integer :: n
  real(8) :: papicount

  ret = gptlevent_name_to_code ('PAPI_FP_OPS', code)
  ret = gptlsetoption (code, 1)
  ret = gptlinitialize ()

  do n=1,niter
    call sub (nval, a, b, c, d, e, f, g, h)
  end do
  write(6,*)'sum=',sum(a) + sum(b) + sum(c) + sum(d) + sum(e) + sum(f) + sum(g) + sum(h)

  ret = gptlget_eventvalue ('vdn', 'PAPI_FP_OPS', 0, papicount)
  write(6,100)'papicount=', papicount,  ' expected ', real(niter)*nval*4
100 format (a, 1p, e12.6, a, e12.6)
  stop
end program main

subroutine sub (nval, a, b, c, d, e, f, g, h)
  use globals
  use gptl

  implicit none

  integer, intent(in) :: nval
  real(prec) :: a(nval)
  real(prec) :: b(nval)
  real(prec) :: c(nval)
  real(prec) :: d(nval)
  real(prec) :: e(nval)
  real(prec) :: f(nval)
  real(prec) :: g(nval)
  real(prec) :: h(nval)

  integer :: i
  integer :: ret

  ret = gptlstart ('vdn')
  do i=1,nval
    a(i) = b(i)*c(i) + d(i)*e(i)
  end do

  do i=1,nval
    f(i) = g(i)*h(i)
  end do
  ret = gptlstop ('vdn')
  return
end subroutine sub
