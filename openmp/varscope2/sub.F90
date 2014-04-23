subroutine sub (iter,addresses)
  implicit none

  integer, intent(in) :: iter
  integer(8), intent(out) :: addresses(*)

  real :: var(1) = 7.
!  real :: var(1)
  integer :: mythread

  integer, external :: omp_get_thread_num
  integer(8), external :: getaddress

  mythread = omp_get_thread_num ()

  addresses(mythread+1) = getaddress (var)
  return
end subroutine sub
