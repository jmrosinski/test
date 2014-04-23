program main
  implicit none

  integer :: iter
  write(6,*) 'Beginning thread stacksize tests in increments of 1 MB'
!$OMP PARALLEL DO
  do iter=1,256
    call sub (iter,1)
  end do
!$OMP END PARALLEL DO
  write(6,*) 'Success'
  stop 0
end program main

recursive subroutine sub (iter,n)
  implicit none

  integer, intent(in) :: iter,n

  integer :: i,j
  integer, parameter :: megabyte = 1048576
  character(len=1) :: bigthing(megabyte)

  write(6,*)'sub: thread=', iter,' entry=',n,' times with 1 MB stack'

  do j=1,megabyte
    bigthing(j) = 'x'
  end do

  call sub (iter,n+1)
  return
end subroutine sub
