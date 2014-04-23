subroutine sub (arr, arr2)
  use dims, only: nip

  implicit none
!  include "interfaces"

  real, intent(in) :: arr(nip), arr2(nip)
  write(6,*)'In sub'
  write(6,*)'arr2=', arr2(:)
  return
end subroutine sub
