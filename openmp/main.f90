program main
  implicit none

  integer, parameter :: niter=128
  integer :: n

!$OMP PARALLEL DO
  do n=1,niter
    call do_work(n,niter)
  end do
  stop
end program main

subroutine do_work (n,niter)
  implicit none
  
  integer, intent(in) :: n
  integer, intent(in) :: niter
  integer(8) :: i
  integer(8) :: ncount
  integer :: nthreads

  integer, external :: omp_get_num_threads

  real(8) :: sum

  nthreads = omp_get_num_threads ()
  ncount = 1.e9 / nthreads
  
  sum = 0.
  do i=1,ncount
    sum = sum + 1./i
  end do
  write(6,*)'sum=',sum
  return
end subroutine do_work
