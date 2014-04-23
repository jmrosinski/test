program main
  use junk
  implicit none

  real :: arr(0:3,2)

  write(6,*) 'main: lower bound of arr=', lbound(arr,1)
  write(6,*) 'main: upper bound of arr=', ubound(arr,1)
  call sub(arr(:,:))
end program main
