program main
  use mymod

  implicit none

  real(4) :: r4, x4
  real(8) :: r8, x8

  x4 = myfunc(r4)
  write(6,*)'myfunc(r4)=', x4
  x8 = myfunc(r8)
  write(6,*)'myfunc(r8)=', x8

  stop
end program main

