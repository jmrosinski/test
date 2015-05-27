subroutine sub (n)
  implicit none

  integer, intent(in) :: n

  real :: arr(3)
  real :: scalar

  call print_address (n, arr, 'arr'//char(0))
  call print_address (n, scalar, 'scalar'//char(0))
end subroutine sub
