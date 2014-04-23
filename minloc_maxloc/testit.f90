program testit
  implicit none

  real :: arr(9)

  arr(:) = 7.
!  arr(9) = 8.
  write(6,*) 'minloc(arr)=', minloc(arr,1)
  write(6,*) 'maxloc(arr)=', maxloc(arr,1)
end program testit
