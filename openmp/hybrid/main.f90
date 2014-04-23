program main
  use mpi
  use gptl

  implicit none

  integer :: ret
  integer :: iam = 0
  integer :: ntask
  integer :: nthread = 1
  integer :: niter, begiter, enditer
  integer :: i
  character(len=MPI_MAX_PROCESSOR_NAME) :: name
  integer :: namelen
  integer, parameter :: totaliter = 1024
  integer, parameter :: required = MPI_THREAD_FUNNELED
  integer :: provided
  real :: arr(100,totaliter)
  real*8 :: totaltime

  integer, external :: omp_get_max_threads

  arr(:,:) = 0.

  call mpi_init_thread (required, provided, ret)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ret)
  call mpi_comm_size (MPI_COMM_WORLD, ntask, ret)
  call mpi_get_processor_name (name, namelen, ret)

  ret = gptlsetoption (gptlcpu, 0)
  ret = gptlinitialize ()

  if (required /= provided) then
    write(6,*) 'required=', required, ' provided=', provided
    call mpi_abort (MPI_COMM_WORLD, -1, ret)
  end if

  if (iam == 0) then
    nthread = omp_get_max_threads ()
    write(6,*)'ntask=', ntask, ' nthread=', nthread
  end if

  write(6,*)'iam=', iam, ' running on:', name(1:namelen)

  if (mod (totaliter,ntask) /= 0) then
    if (iam == 0) then
      write(6,*) 'ntask=', ntask, ' must divide evenly into totaliter=', totaliter
    end if
    call mpi_abort (MPI_COMM_WORLD, -1, ret)
  end if

  niter = totaliter / ntask
  begiter = iam * niter + 1
  enditer = begiter + niter - 1

  ret = gptlstart ('total')

!$OMP PARALLEL DO SHARED (ARR)
  do i=begiter,enditer
    call do_work (arr(1,i))
  end do
!$OMP END PARALLEL DO

  ret = gptlstop ('total')
  ret = gptlpr (iam)
  ret = gptlget_wallclock ('total', 0, totaltime)
  write(6,*)'iam=', iam, ' total time=', totaltime

  call mpi_finalize (ret)
  stop
end program main

subroutine do_work (arr)
  use gptl

  implicit none

  real, intent(out) :: arr(100)

  integer :: i, j
  integer :: ret

  ret = gptlstart ('do_work')
  do i=1,100
    do j=1,10000
      arr(i) = arr(i) + exp(1./(i+j))
    end do
  end do
  ret = gptlstop ('do_work')

  return
end subroutine do_work
