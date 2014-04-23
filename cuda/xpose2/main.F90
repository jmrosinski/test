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

  character(len=32) :: str

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

  if (.true.) then
  arrout(:,:,:) = -999.
  str = 'xpose_if'
  write(6,*)'calling ', trim(str)
  ret = gptlstart (trim(str))
  call xpose_if (arrin, arrout, m, n, mm, nn, niter)
  ret = gptlstop (trim(str))
  call checkandprint (trim(str), m, n, niter, arrin, arrout)

  arrout(:,:,:) = -999.
  str = 'xpose_min'
  write(6,*)'calling ', trim(str)
  ret = gptlstart (trim(str))
  call xpose_min (arrin, arrout, m, n, mm, nn, niter)
  ret = gptlstop (trim(str))
  call checkandprint (trim(str), m, n, niter, arrin, arrout)
  end if

  arrout(:,:,:) = -999.
  str = 'xpose_direct'
  write(6,*)'calling ', trim(str)
  ret = gptlstart (trim(str))
  call xpose_direct (arrin, arrout, m, n, niter)
  ret = gptlstop (trim(str))
  call checkandprint (trim(str), m, n, niter, arrin, arrout)

  arrout(:,:,:) = -999.
  str = 'xpose_direct_stridedwrite'
  write(6,*)'calling ', trim(str)
  ret = gptlstart (trim(str))
  call xpose_direct_stridedwrite (arrin, arrout, m, n, niter)
  ret = gptlstop (trim(str))
  call checkandprint (trim(str), m, n, niter, arrin, arrout)

! Use mm as chunksize
  if (mod (m,mm) == 0) then
    nchunks = m/mm
  else
    nchunks = m/mm + 1
  end if

  deallocate (arrout)
  allocate (arrout2(n,mm,nchunks,niter))
  arrout2(:,:,:,:) = -999.

! Just declares space for arrout2
#ifdef GPU
  call copyin2 (n, mm, nchunks, niter)
#endif

  write(6,*)'calling xpose_chunks_stridedread'
  ret = gptlstart ('xpose_chunks_stridedread')
  call xpose_chunks_stridedread (arrin, arrout2, m, n, mm, nchunks, niter)
  ret = gptlstop ('xpose_chunks_stridedread')
  call checkandprint2 ('xpose_chunks_stridedread', m, n, mm, nchunks, niter, arrin, arrout2)

  write(6,*)'calling xpose_chunks_stridedwrite'
  ret = gptlstart ('xpose_chunks_stridedwrite')
  call xpose_chunks_stridedwrite (arrin, arrout2, m, n, mm, nchunks, niter)
  ret = gptlstop ('xpose_chunks_stridedwrite')
  call checkandprint2 ('xpose_chunks_stridedwrite', m, n, mm, nchunks, niter, arrin, arrout2)

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
