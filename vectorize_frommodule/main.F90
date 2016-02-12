program main
  use funcs, only: vecfunc
  implicit none

  integer, parameter :: nelem = 64
  real :: x(nelem)
  real :: y(nelem)
  real :: z(nelem)
  integer :: i

  x(:) = 1.
!DIR$VECTOR ALWAYS
  do i=1,nelem
    y(i) = vecfunc(x(i))
  end do
!DIR$VECTOR ALWAYS
  z(:) = vecfunc(y(:))
  write(6,*)'z=',z(:)
end program main
