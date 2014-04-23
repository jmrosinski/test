program main
  implicit none

  real, allocatable :: arr(:)

  allocate (arr(1))
  call teststack ()
  call testalloc (arr)
  stop
end program main

subroutine testalloc (arr)
  implicit none

  real, intent(in) :: arr(*)

  write(6,*)'Testing reference arr(2) when only arr(1) was allocated...'
  write(6,*)'arr(2)=', arr(2)
  return
end subroutine testalloc

subroutine teststack ()
  implicit none

  real :: stackarr(1)

  call sub (stackarr)
  return
end subroutine teststack

subroutine sub (stackarr)
  implicit none

  real, intent(in) :: stackarr(*)

  write(6,*)'Testing reference arr(2) when only stackarr arr(1) was defined...'
  write(6,*)'stackarr(2)=', stackarr(2)
  return
end subroutine sub

