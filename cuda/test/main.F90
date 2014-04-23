program main
  use gptl
! Test program transposes an array on the GPU and sees that it's correct
  implicit none

!  integer, parameter :: nip = 10
!  integer, parameter :: maxtpb = 8 ! max threads per block (see output from deviceQuery)
!  integer :: nz = 2

  integer, parameter :: m = 10242
  integer, parameter :: n = 192

  integer, parameter :: mm = 64
  integer, parameter :: nn = 96

  integer, parameter :: niter=10

  real, allocatable :: arrin(:,:,:)
  real, allocatable :: arrout(:,:,:)
  real, allocatable :: arrout2(:,:,:,:)
  integer :: i, j, iter
  integer :: nchunks
  integer :: ret
  integer, external :: omp_get_max_threads

  allocate (arrin(m,n,niter))
  allocate (arrout(n,m,niter))

  do iter=1,niter
    do j=1,n
      do i=1,m
! Create dummy data
        arrin(i,j,iter) = 1./(i+j+iter)
      end do
    end do
  end do

#ifdef GPU
  write(6,*)'GPU code'
  call copyin (arrin, m, n, niter)
#else
  write(6,*)'CPU code: num threads=', omp_get_max_threads()
#endif
  write(6,*)'subarray dimensions: mm=', mm, ' nn=', nn

  ret = gptlsetutr (gptlnanotime)
  ret = gptlinitialize ()

  arrout(:,:,:) = -999.
  write(6,*)'calling xpose_direct_stridedwrite'
  ret = gptlstart ('zzz')
  call xpose_direct_stridedwrite (arrin, arrout, m, n, niter)
  ret = gptlstop ('zzz')
  call checkandprint ('zzz', m, n, niter, arrin, arrout)

  ret = gptlpr (0)

  stop 0
end program main

subroutine checkandprint (str, m, n, niter, arrin, arrout)
  use gptl
  implicit none

  character(len=*), intent(in) :: str
  integer, intent(in) :: m, n, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(inout) :: arrout(n,m,niter)

  integer :: i, j, iter, ret

  real*8 :: val
  real :: rate

#ifdef GPU
! Copy arrout from the GPU to the CPU
  call copyout (arrout, m, n, niter)
#endif

! Verify correctness of results
  do iter=1,niter
    do j=1,n
      do i=1,m
        if (arrout(j,i,iter) /= arrin(i,j,iter)) then
          write(6,*) 'BAD OUTPUT: i,j,iter,arrin,arrout=', i, j, iter, arrin(i,j,iter), arrout(j,i,iter)
          return
        end if
      end do
    end do
  end do

  write(6,*) str,' did the right thing!'

  ret = gptlget_wallclock (str, 0, val)
! 2 is for reading and writing
! 4 is bytes per word
  rate = niter * m * n * 2. * 4. / (1.e6 * val)
  write(6,*)'xpose BW = ', rate, ' MB/s'
  write(6,*)

  return
end subroutine checkandprint

subroutine checkandprint2 (str, m, n, chunksize, nchunks, niter, arrin, arrout2)
  use gptl
  implicit none

  character(len=*), intent(in) :: str
  integer, intent(in) :: m, n, chunksize, nchunks, niter
  real, intent(in) :: arrin(m,n,niter)
  real, intent(inout) :: arrout2(n,chunksize,nchunks,niter)

  integer :: i, j, iter, ret
  integer :: ii, c

  real*8 :: val
  real :: rate

#ifdef GPU
! Copy arrout2 from the GPU to the CPU
  call copyout2 (arrout2, n, chunksize, nchunks, niter)
#endif

! Verify correctness of results
  do iter=1,niter
    do c=1,nchunks
      do ii=1,chunksize
        i = (c-1)*chunksize + ii
        if (i <= m) then
          do j=1,n
            if (arrout2(j,ii,c,iter) /= arrin(i,j,iter)) then
              write(6,*) 'BAD OUTPUT: i,j,iter,ii,c,arrin,arrout=', &
                                      i,j,iter,ii,c,arrin(i,j,niter), arrout2(j,ii,c,iter)
              return
            end if
          end do
        end if
      end do
    end do
  end do

  write(6,*) str,' did the right thing!'

  ret = gptlget_wallclock (str, 0, val)
  rate = niter * m * n * 2. * 4. / (1.e6 * val)
  write(6,*)'xpose BW = ', rate, ' MB/s'
  write(6,*)

  return
end subroutine checkandprint2
