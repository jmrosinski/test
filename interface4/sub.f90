subroutine sub (arr)
  use dims, only: nip

  implicit none

  include "interfaces"

  real, intent(in) :: arr(nip)
  write(6,*)'In sub'
  return
end subroutine sub
