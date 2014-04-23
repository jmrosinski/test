program main
  use solve
  implicit none

#include <gptl.inc>

  integer :: n = 96       ! order of matrix: this is the value used by NIM
!  integer :: n = 296       ! order of matrix
  integer :: i
  integer :: ret
  integer :: code

  ret = gptlevent_name_to_code ('PAPI_FP_OPS', code)
  ret = gptlsetoption (code, 1)
  ret = gptlinitialize ()
  do i=1,10000
    call tridiagsolve (n, i)
  end do

  ret = gptlpr (0)
end program main
