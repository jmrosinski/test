subroutine sub (a, b, maxelem)
  integer, intent(in) :: maxelem
  real, intent(inout) :: a(maxelem)
  real, intent(inout) :: b(maxelem)
  real :: sum(maxelem)

  a(:) = 2.1 + 3.3*a(:)
  b(:) = 5.5 + 2.2*b(:)
end subroutine sub
