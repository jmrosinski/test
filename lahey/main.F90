program main
  use infnan

  implicit none

  type gfs
    real, allocatable :: arr(:)
  end type gfs

  integer, parameter :: nelem = 100
  type(gfs) :: thing
  real, allocatable :: arr2(:)

  allocate(thing%arr(nelem))
  allocate(arr2(nelem))

!  arr(:) = 1.e38
  write(6,*)'calling sub with struct'
  call sub(thing%arr, nelem)
  write(6,*)'calling sub with arr2'
  call sub(arr2, nelem)
  stop 0
end program main
