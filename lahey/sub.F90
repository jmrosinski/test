subroutine sub (arr, nelem)
  implicit none

  integer, intent(in) :: nelem
  real, intent(inout) :: arr(nelem)
  
  integer :: i
  real :: sum

  sum = 0.

  write(6,*)'sub: input arr=', arr(:)
  do i=1,nelem
    sum = sum + arr(i)
  end do
  write(6,*)'sum=', sum
  return
end subroutine sub
