program main
  use dims, only: nip
  implicit none

  include "interfaces"

  real, allocatable :: arr(:)

  allocate (arr(nip))
  call sub(arr)
end program main
