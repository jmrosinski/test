program junk
  implicit none

  integer :: n
  external :: omp_set_num_threads

  call omp_set_num_threads (2)
!$OMP PARALLEL DO
  do n=1,2
    call sub()
  end do
end program junk

subroutine sub()
  implicit none

  type thing
     integer, pointer :: guts => null()
  end type thing

  type (thing) :: v1                ! Will get multiple copies
  integer, pointer :: v2 => null()  ! only 1 copy due to =>null()
  integer :: mythread
  integer, target, save :: iarr(0:1) = (/0,1/)
  integer, external :: omp_get_thread_num

  mythread = omp_get_thread_num()
  call sleep(mythread)      ! guarantee that thread 1 gets here last
  v1%guts => iarr(mythread)
  v2 => iarr(mythread)
  call sleep(3-mythread)    ! guarantee that thread 0 gets here last
! If there are multiple OMP copies, guts and v2 will each match mythread
! If there is only 1 OMP copy, thread 0 and thread 1 will both print 1
  write(6,*)'mythread=',mythread,' guts=',v1%guts,' v2=',v2
end subroutine sub
