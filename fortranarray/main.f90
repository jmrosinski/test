program main
  use subs
  implicit none

  real :: arr(9:19)

  call submod (arr)
  call subimp (arr)
  stop
end program main
