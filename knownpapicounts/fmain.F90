program fmain
  implicit none

#include <gptl.inc>

  integer, parameter :: nelem = 1000000
  integer :: i, n, ret
  integer :: code
  real(8) :: sum(8)
  real(8) :: x, y, z

  ret = gptlevent_name_to_code ('PAPI_FP_OPS', code)
  if (ret /= 0) then
    write(6,*)'Error in gptleventname_to_code: ret=', ret
    stop 999
  end if

  ret = gptlsetoption (code, 1)
  if (ret /= 0) then
    write(6,*)'Error in gptlsetoption: ret=', ret
    stop 999
  end if 

  ret = gptlinitialize ()

  sum(:) = 0.
  do n=1,10
    ret = gptlstart ('loop')
    do i=1,nelem
      x = i
      y = 0.1*i
      sum(1) = sum(1) + 1.1*x + 1.1*y
      sum(2) = sum(2) + 2.2*x + 2.2*y
      sum(3) = sum(3) + 3.3*x + 3.3*y
      sum(4) = sum(4) + 4.4*x + 4.4*y
      sum(5) = sum(5) + 5.5*x + 5.5*y
      sum(6) = sum(6) + 6.6*x + 6.6*y
      sum(7) = sum(7) + 7.7*x + 7.7*y
      sum(8) = sum(8) + 8.8*x + 8.8*y
    end do
    ret = gptlstop ('loop')
  end do
  ret = gptlpr (0)
  stop
end program fmain
