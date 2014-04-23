program vectest
  use gptl
  implicit none

  integer, parameter :: niter = 1000
  integer, parameter :: veclen = 16
  integer :: i, ret
  logical :: custom = .false.
  real :: x(veclen)
  real :: y(veclen)
  real :: z(veclen)
  
  ret = gptlinitialize ()

  do i=1,veclen
    x(i) = i
    y(i) = 1./i
  end do

  ret = gptlstart ('sub')
  call sub (custom, veclen, x, y, z, niter)
  ret = gptlstop ('sub')
  ret = gptlpr (0)

  stop
end program vectest

