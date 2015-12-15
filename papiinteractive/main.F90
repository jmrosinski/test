program main
  use omp
  use gptl
  implicit none

  integer, parameter :: maxelem = 1000000
  real :: a(maxelem), b(maxelem)
  integer :: ncounter = 0
  integer :: ret
  character(len=32) :: counter
  integer :: icounter
  logical :: done = .false.
  integer :: nthreads
  
  do while (.not. done)
    write(6,*)'Enter GPTL or PAPI counter to enable'
    read(5,*) counter
    ret = gptl_papi_event_name_to_code (trim(counter), icounter)
    if (ret == 0) then
      write(6,*)'counter for ',trim(counter),' =',icounter
      ret = gptlenable (icounter, 1)
      if (ret == 0) then
        write(6,*)'Successfully enabled counter=',trim(counter)
        n = n + 1
        countersave(ncounter) = counter
      else
        write(6,*)'Failed to enable counter=',trim(counter)
      end if
    else
      write(6,*)'Counter not found for ',trim(counter)
    end if
    write(6,*)'Enter <CR> to enable more counters, anything else when done'
    read(5,*) ans
    done = ans == ' '
  end do

  write(6,*)'Enter number of threads or <CR> for no threading'
  read(5,*)nthreads
  ret = omp_set_num_threads (nthreads)
  ret = gptlinitialize ()

  a(:) = .1
  b(:) = .2

!$OMP PARALLEL DO FIRSTPRIVATE(a,b)
  do n=1,nthreads
    ret = gptlstart('sub')
    call sub (a, b)
    ret = gptlstop('sub')
  end do

  ret = gptlget_eventvalue ('sub', 
end program main

subroutine sub (a, b, maxelem)
  integer, intent(in) :: maxelem
  real, intent(inout) :: a(maxelem)
  real, intent(inout) :: b(maxelem)
  real :: sum(maxelem)

  a(:) = 2.1 + 3.3*a(:)
  b(:) = 5.5 + 2.2*b(:)
end subroutine sub
