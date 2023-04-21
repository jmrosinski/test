program basic
  use gptl
  use gptl_acc
  
  implicit none

  integer :: n, ret
  integer :: zero = 0

  ret = gptlstart (0)
!$acc parallel private(ret) copyin(n)
  do n=1,2
    ret = gptlstart_gpu (n)
    ret = gptlstop_gpu (n)
  end do
!$acc end parallel
  ret = gptlstop (0)
  ret = gptlcudadevsync ()
  stop 0
end program basic
