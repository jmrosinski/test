program main
  use gptl
  implicit none

  integer :: ret
  real :: x = 0.
  
  ret = gptlinitialize ()
  ret = gptlprint_memusage ('before sub')
  call sub (x, 1000, 1000, 100)
  ret = gptlprint_memusage ('after sub')
  stop
end program main

