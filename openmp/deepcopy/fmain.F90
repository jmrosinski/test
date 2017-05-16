program main
  use defs
  implicit none

  type (tobecopied) :: testit
  integer :: n

  allocate (testit%x(9))
  allocate (testit%y(9))
  do n=1,9
    testit%x(n) = n
    testit%y(n)%z = n
  end do

!$OMP PARALLEL DO FIRSTPRIVATE (testit)
  do n=1,4
    call sub (n, testit)
  end do
!$OMP END PARALLEL DO
end program main
