program main
  use gptl

  implicit none

  integer, parameter :: niter = 10242  ! G5
  integer :: n                         ! loop index
  integer :: nthreads                  ! number of threads
  integer :: whodoneit(0:niter-1)      ! records thread responsible for iteration
  integer, allocatable :: kount(:)     ! records number of iters done by each thread
  integer :: ret

  integer, external :: omp_get_max_threads

  nthreads = omp_get_max_threads ()
  allocate (kount(0:nthreads-1))
  kount(:) = 0

  ret = gptlinitialize ()

!$OMP PARALLEL DO PRIVATE (ret) SCHEDULE (runtime)
  do n=0,niter-1
    ret = gptlstart ('balanced')
    call load_balanced_sub (niter, n, whodoneit, kount)
    ret = gptlstop ('balanced')
  end do

  call printstats ('balanced')
  kount(:) = 0

!$OMP PARALLEL DO PRIVATE (ret) SCHEDULE (runtime)
  do n=0,niter-1
    ret = gptlstart ('unbalanced')
    call load_unbalanced_sub (n, whodoneit, kount)
    ret = gptlstop ('unbalanced')
  end do

  call printstats ('unbalanced')

  ret = gptlpr (0)

  stop

contains
  subroutine printstats (string)
    character(len=*), intent(in) :: string

    integer :: i
    integer :: ret
    integer :: prvwho  ! who did the previous iteration
    real*8 :: value
    
    write(6,*) 'stats for ', string, ':'
    do i=0,nthreads-1
      ret = gptlget_wallclock (string, i, value)
      write(6,'(a,i3,a,i4,1p,e9.2)') 'iterations, total time for thread ', i, ':', kount(i), value
    end do

    write(6,*) 'distribution info:'
    prvwho = -1     ! initialize so 1st is false
    do i=0,niter-1
      if (whodoneit(i) /= prvwho) then
        write(6,'(a,i6,a,i3)') 'iteration ', i, ' was done by thread ', whodoneit(i)
      end if
      prvwho = whodoneit(i)
    end do

    return
  end subroutine printstats
end program main
