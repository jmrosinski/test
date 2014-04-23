program main
  implicit none

  real :: uvals(2) = (/1.,2./)
  real :: vvals(2) = (/10.,20./)

  call sub ('u', uvals)
  call sub ('v', vvals)

  uvals(:) = 1.
  vvals(:) = 1.
  call sub ('u', uvals)
  call sub ('v', vvals)

  stop
end program main

subroutine sub (name, arr)
  implicit none

  character(len=*), intent(in) :: name
  real, intent(in) :: arr(2)

  real, allocatable, save :: u(:)
  real, allocatable, save :: v(:)

  if (name == 'u') then
    if (allocated (u)) then
      write(6,*)'Error: got passed u and its array is already allocated'
      stop 999
    else
      allocate (u(2))
      write(6,*)'Storing u for later use'
      u(:) = arr(:)
    end if
  else if (name == 'v') then
    if (allocated (v)) then
      write(6,*)'Error: got passed v and its array is already allocated'
      stop 999
    else
      allocate (v(2))
      write(6,*)'Storing v for later use'
      v(:) = arr(:)
    end if
  end if

  if (allocated (u) .and. allocated (v)) then
    write(6,*) 'Have both u and v: sum=', sum(u(:)) + sum(v(:))
    deallocate (u, v)
  end if

  return
end subroutine sub
