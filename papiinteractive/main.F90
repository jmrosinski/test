program main
  use gptl
  implicit none

  integer, parameter :: maxelem = 1000000
  real, allocatable :: a(:,:), b(:,:)
  integer :: ncounter = 0
  integer :: ret
  character(len=32) :: counter
  character(len=32) :: countersave(9)
  integer :: icounter
  logical :: done = .false.
  integer :: nthreads
  integer :: n, t
  character(len=1) :: ans
  real*8 :: val

  integer, external :: omp_set_num_threads
  
  do while (.not. done)
    write(6,*)'Enter GPTL or PAPI counter to enable'
    read(5,*) counter
    ret = gptlevent_name_to_code (trim(counter), icounter)
    if (ret == 0) then
      write(6,*)'counter for ',trim(counter),' =',icounter
      ret = gptlsetoption (icounter, 1)
      if (ret == 0) then
        write(6,*)'Successfully enabled counter=',trim(counter)
        ncounter = ncounter + 1
        countersave(ncounter) = counter
      else
        write(6,*)'Failed to enable counter=',trim(counter)
      end if
    else
      write(6,*)'Counter not found for ',trim(counter)
    end if
    write(6,*)'Enter "d" when done, any other letter to enable more counters'
    read(5,*) ans
    done = ans == 'd'
  end do

  write(6,*)'Enter number of threads or <CR> for no threading'
  read(5,*)nthreads
  ret = omp_set_num_threads (nthreads)
  ret = gptlinitialize ()

  allocate(a(maxelem,nthreads))
  allocate(b(maxelem,nthreads))
  a(:,:) = .1
  b(:,:) = .2

!$OMP PARALLEL DO PRIVATE(ret)
  do n=1,nthreads
    ret = gptlstart('sub')
    call sub (a(1,n), b(1,n), maxelem)
    ret = gptlstop('sub')
  end do

  do n=1,ncounter
    do t=0,nthreads-1
      if (gptlget_eventvalue ('sub', trim(countersave(n)), t, val) == 0) then
        write(6,*)'thread=',t,' counter=',trim(countersave(n)),' val=', val
      else
        write(6,*)'thread=',t,' Failure from gptlget_eventvalue for counter=',trim(countersave(t))
      end if
    end do
  end do
  write(6,*)'calling gptlpr'
  ret = gptlpr (0)
end program main

