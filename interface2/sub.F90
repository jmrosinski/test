subroutine sub (arr)
  use sizes
  use external_interfaces

  implicit none

  real, intent(out) :: arr(ims:ime)
  
  write(6,*)'Inside sub'
  arr(:) = 0.
end subroutine sub
