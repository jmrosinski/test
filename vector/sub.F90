subroutine sub (custom, veclen, x, y, z, niter)
  implicit none

  logical, intent(in) :: custom
  integer, intent(in) :: veclen
  real, intent(in) :: x(veclen)
  real, intent(in) :: y(veclen)
  real, intent(inout) :: z(veclen)
  integer, intent(in) :: niter

  integer :: i, iter

  real, external :: slowpowf
  
  do iter=1,niter
!DIR$ VECTOR ALWAYS
    do i=1,veclen
      if (custom) then
        z(i) = x(i)**y(i)
      else
        z(i) = slowpowf (x(i),y(i))
      end if
    end do
  end do

  return
end subroutine sub

