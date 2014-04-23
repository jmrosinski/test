module sub2inmod
  implicit none
  save
contains
  subroutine sub2 (iter,addresses)

    integer, intent(in) :: iter
    integer(8), intent(out) :: addresses(*)
    
    real :: var
    integer :: mythread

    integer, external :: omp_get_thread_num
    integer(8), external :: getaddress

    mythread = omp_get_thread_num ()
    
    addresses(mythread+1) = getaddress (var)
    return
  end subroutine sub2
end module sub2inmod
