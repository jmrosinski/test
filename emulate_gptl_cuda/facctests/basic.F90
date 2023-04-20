program basic
  use gptl
  use gptl_acc
  
  implicit none

  integer :: n, ret
  integer :: zero = 0

  ret = gptlstart (0)
!$acc parallel private(ret) copyin(zero)
  ret = gptlstart_gpu (zero)
  ret = gptlstop_gpu (zero)
!$acc end parallel
  ret = gptlstop (0)
  ret = gptlcudadevsync ()
  stop 0
end program basic
