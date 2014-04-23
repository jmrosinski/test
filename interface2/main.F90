program main
  use sizes
  use external_interfaces
  
  real, allocatable :: arr(:)

  allocate (arr(ims:ime))
  call sub (arr)
end program main
