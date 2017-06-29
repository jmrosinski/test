program main
  implicit none

  integer :: n
  integer :: mythread
  integer, external :: omp_get_thread_num

!$omp parallel do ordered private (mythread)
  do n=1,20
    mythread = omp_get_thread_num()
    write(6,*) 'mythread=',mythread,' entered parallel region n=', n
!$omp ordered 
    write(6,*) 'mythread=',mythread,' inside ordered region n=', n
!$omp end ordered
  end do
!$omp end parallel do
end program main
