program main
  use gptl
  use subs, only: sub
  implicit none

  integer :: niter
  integer :: ret
  integer :: nthreads ! number of threads
  integer :: n        ! loop index
  integer :: t        ! thread index
  real    :: arr(100)
  real*8 :: gutssum, guts, total, ohd
  integer, external :: omp_get_thread_num, omp_get_max_threads

  ret = gptlsetutr (gptlnanotime)
  ret = gptlinitialize ()

  do while (.true.)
    write(6,*)'Enter number of threads:'
    read(5,*)nthreads
    write(6,*)'Enter number of parallel iterations:'
    read(5,*)niter
    write(6,*)'niter=',niter
! Remove GPTL startup overhead associated with first call to a routine
    ret = gptlstart('total')
    ret = gptlstop('total')
    ret = gptlstart('guts')
    ret = gptlstop('guts')
    ret = gptlreset()

    call omp_set_num_threads (nthreads)
    ret = gptlstart ('total')
!$OMP PARALLEL DO PRIVATE(ret,arr)
    do n=1,niter
      ret = gptlstart ('guts')
      call sub (n, arr)
      ret = gptlstop ('guts')
    end do
    ret = gptlstop ('total')

    gutssum = 0.
    do t=1,nthreads
      ret = gptlget_wallclock ('guts', t-1, guts)
      gutssum = gutssum + guts
    end do
    ret = gptlget_wallclock ('total', 0, total)
    ohd = total - gutssum
    write(6,100) total, gutssum
100 format ('total=',1p,e8.1,' gutssum=',e8.1)
    write(6,101) ohd
101 format ('overhead as residual=',1p,e8.1)
    ret = gptlpr (0)
    write(6,*)'see timing.0 for timing overhead'
    ret = gptlreset ()
  end do
end program main
