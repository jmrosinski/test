program main
  use sub2inmod
  implicit none

  integer :: niter
  integer :: iter
  integer(8), allocatable :: addresses(:)
  integer(8) :: lastsameval, lastdifferval
  integer :: lastsame, lastdiffer
  logical :: allsame, alldiffer

  integer, external :: omp_get_max_threads

  niter = omp_get_max_threads ()
  if (niter < 2) then
    write(6,*)'This program is only useful if nthreads > 1'
    stop
  end if

  allocate (addresses(niter))

!$OMP PARALLEL DO
  do iter=1,niter
    call sub (iter, addresses)
  end do
!$OMP END PARALLEL DO

!$OMP PARALLEL DO
  do iter=1,niter
    call sub2 (iter, addresses)
  end do
!$OMP END PARALLEL DO

  allsame = .true.
  alldiffer = .true.
  do iter=2,niter
    if (addresses(iter) == addresses(1)) then
      alldiffer = .false.
      lastsame = iter
      lastsameval = addresses(iter)
    else
      allsame = .false.
      lastdiffer = iter
      lastdifferval = addresses(iter)
    end if
  end do

  if (allsame) then
    write(6,*)'niter=',niter,' all addresses matched:',lastsameval
  end if
  if (alldiffer) then
    write(6,*)'niter=',niter,' all addresses differed:',addresses(1),lastdifferval
  end if
  if (.not. (allsame .or. alldiffer)) then
    write(6,*)'ONLY SOME ADDRESSES DIFFERED!!!!!:'
    do iter=1,niter
      write(6,*)addresses(iter)
    end do
  end if

  stop
end program main
