program main
  implicit none

  write(6,*) 'Beginning stacksize tests in increments of 100 MB'
  call sub (1)
  write(6,*) 'Success'
  stop 0
end program main

recursive subroutine sub (n)
  implicit none

  integer, intent(in) :: n

  integer :: i,j
  integer, parameter :: hundredmb = 104857600
  real :: bigthing(hundredmb/4)

  if (mod(n,1) == 0) then
    write(6,*)'sub: entry=',n,' times with 100 MB stack'
  end if

  do j=1,hundredmb/4
    bigthing(j) = 7.
  end do

  if (n == 57) then
    write(6,*)'bigthing=', bigthing(hundredmb/4)
  end if
  call sub (n+1)
  return
end subroutine sub
