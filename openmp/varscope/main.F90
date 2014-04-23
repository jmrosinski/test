program main
  implicit none

  integer :: niter
  integer :: iter
  real :: var1
  integer, external :: omp_get_max_threads

#ifdef DYNAMIC
  real, allocatable :: var2(:)
  allocate (var2(1))
#else
  real :: var2(1)
#endif

  niter = omp_get_max_threads ()

!$OMP PARALLEL DO
  do iter=1,niter
    call sub (iter)
  end do
!$OMP END PARALLEL DO

#ifdef DYNAMIC
  deallocate (var2)
#endif
  call flush(6)
  stop
end program main
