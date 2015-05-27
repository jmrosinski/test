program main
  implicit none

  integer :: n

!$OMP PARALLEL DO
  do n=1,2
    call sub (n)
  end do
end program main
