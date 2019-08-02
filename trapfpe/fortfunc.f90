function fortfunc (x, y) result (z)
  implicit none

  real :: x,y
  real :: z

  z = x / y
end function fortfunc

function ifortfunc_div (x, y) result (z)
  implicit none

  integer :: x,y
  integer :: z

  z = x / y
end function ifortfunc_div


function ifortfunc_mult (x, y) result (z)
  implicit none

  integer :: x,y
  integer :: z

  z = x * y
end function ifortfunc_mult
