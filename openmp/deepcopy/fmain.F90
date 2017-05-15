program main
  use defs
  implicit none

  type (tobecopied) :: testit
  integer :: n

  allocate (testit%x(9))
  do n=1,9
    testit%x(n) = n
  end do

!$OMP PARALLEL DO PRIVATE (testit)
  do n=1,4
    call sub (n, testit)
  end do
end program main
