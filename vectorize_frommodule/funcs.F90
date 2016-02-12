module funcs
  implicit none

contains
  elemental real function vecfunc (x)
    real, intent(in) :: x
    vecfunc = x + 1.
  end function vecfunc
end module funcs
