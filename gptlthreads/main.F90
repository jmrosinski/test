program main
  use mpi
  use gptl
  implicit none

  integer :: ret
  character(len=MPI_MAX_PROCESSOR_NAME) :: mynode
  character(len=64) :: region
  integer :: iam
  integer :: n
  integer :: resultlen
  integer :: ips, ipe, ipn
  integer, parameter :: nnames = 2
  character(len=64) :: names(nnames) = (/'threading','next'/)

  call mpi_init (ret)
  call mpi_comm_rank (MPI_COMM_WORLD, iam, ret)
  call mpi_get_processor_name (mynode, resultlen, ret)

  write(6,'(a,i0,a,a)') 'Rank=', iam, ' is running on node=', trim(mynode)

  ret = gptlinitialize ()
  ret = gptlstart ('total')

  ips = 1
  ipe = 10242

!$OMP PARALLEL DO
  do ipn=ips,ipe
    call threadedsub (ipn)
  end do

  do n=1,nnames
    if (iam == 0) then
      write(6,*)'call sub(',trim(names(n)),')'
    end if
    call sub (ips, ipe, trim(names(n)))
  end do

!$OMP PARALLEL DO
  do ipn=ips,ipe
    call threadedsub (ipn)
  end do

  ret = gptlstop ('total')
  ret = gptlpr (iam)
  ret = gptlpr_summary (MPI_COMM_WORLD)
  call mpi_finalize (ret)
end program main

subroutine sub (ips, ipe, name)
  use gptl
  implicit none

  integer, intent(in) :: ips, ipe
  character(len=*), intent(in) :: name
  integer :: ipn
  integer :: ret
  
!$OMP PARALLEL DO PRIVATE (ret)
  do ipn=ips,ipe
    ret = gptlstart (name)
    ret = gptlstop (name)
  end do
!$OMP END PARALLEL DO

  return
end subroutine sub

subroutine threadedsub (ipn)
  implicit none

  integer, intent(in) :: ipn

  real :: arr(ipn)

  arr(:) = 0.
  call do_nothing(arr, ipn)
  return
end subroutine threadedsub

subroutine do_nothing (arr, ipn)
  implicit none

  integer, intent(in) :: ipn
  real, intent(inout) :: arr(*)

  if (ipn < 0) then
    write(6,*)'arr=', arr(-ipn)
  end if
  return
end subroutine do_nothing
