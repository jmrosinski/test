module mymod
  implicit none

  interface myfunc
    module procedure myfunc_r4, myfunc_r8
  end interface

CONTAINS

  real(4) function myfunc_r4(arg)
    real(4), intent(in) :: arg
    myfunc_r4 = 4.
  end function myfunc_r4

  real(8) function myfunc_r8(arg)
    real(8), intent(in) :: arg
    write(6,*)'inside myfunc_r8 arg=',arg
    myfunc_r8 = 8.
  end function myfunc_r8
end module mymod
