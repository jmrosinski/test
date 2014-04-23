program main
  use gptl
  implicit none

  integer, external :: omp_get_thread_num
  integer, external :: omp_get_max_threads

  integer :: n
  integer :: mythread
  integer :: nthreads
  integer, parameter :: niter = 1024200
  integer :: ret
  integer*8, allocatable :: loophandle(:)
  integer*8 :: whole_loophandle
  real*8 :: wc, wcmin, wcmax, wctot, wcsum
  integer :: tmin, tmax

  nthreads = omp_get_max_threads()
  allocate(loophandle(0:nthreads-1))
  loophandle(:) = 0
  whole_loophandle = 0
  ret = gptlsetutr (gptlnanotime)
  ret = gptlinitialize ()

  write(6,*)'Warming up ', nthreads, ' threads'
  ret = gptlstart_handle ('whole_loop',whole_loophandle)
  ret = gptlstop_handle ('whole_loop',whole_loophandle)
!$OMP PARALLEL DO PRIVATE(ret,mythread)
  do n=1,nthreads
    mythread = omp_get_thread_num()
    ret = gptlstart_handle ('loop_iter',loophandle(mythread))
    ret = gptlstop_handle ('loop_iter',loophandle(mythread))
  end do
  ret = gptlreset()

! Time the whole loop and its iterations
  ret = gptlstart_handle ('whole_loop',whole_loophandle)
!$OMP PARALLEL DO PRIVATE(mythread)
  do n=1,niter
    mythread = omp_get_thread_num()
    ret = gptlstart_handle('loop_iter',loophandle(mythread))
    ret = gptlstop_handle('loop_iter',loophandle(mythread))
  end do
  ret = gptlstop_handle ('whole_loop',whole_loophandle)

! Estimate OMP overhead as whole_loop_cost minus sum_of_iterations
  wcsum = 0.
  wcmax = -1.e38
  wcmin = +1.e38
  do n=0,nthreads-1
    ret = gptlget_wallclock ('loop_iter', n, wc)
    wcsum = wcsum + wc
    if (wc > wcmax) then
      tmax = n
      wcmax = wc
    end if
    if (wc < wcmin) then
      tmin = n
      wcmin = wc
    end if
    wcmin = min(wc, wcmin)
  end do
  ret = gptlget_wallclock ('whole_loop', 0, wctot)

  write(6,*) 'Cost of loop=', wctot
  write(6,*) 'Cost of inner loop=', wcsum
  write(6,*) 'Residual cost of threading overhead + GPTL overhead + omp_get_thread_num=', wctot-wcsum
  write(6,*) 'tmin,wcmin=', tmin, wcmin
  write(6,*) 'tmax,wcmax=', tmax, wcmax
  ret = gptlpr(0)

  stop
end program main
