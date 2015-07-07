program main
  use gptl
  use parms, only: nz, chunksize
  use subs, only: sub_automatic, sub_alloc, sub_stack

  implicit none

  integer, parameter :: nip = 10242
  integer, parameter :: niter = 1000
  integer :: nthreads = 1
  integer :: n, iter, npts
  integer :: ret
  real :: outval

#ifdef _OPENMP
  integer, external :: omp_get_max_threads

  nthreads = omp_get_max_threads ()
#endif

  ret = gptlinitialize ()

  do iter=1,niter
!$OMP PARALLEL DO PRIVATE (npts, outval, ret)
    do n=1,nip,chunksize
      npts = min (chunksize, nip-n+1)
      ret = gptlstart ('automatic')
      call sub_automatic (npts, nz, chunksize, outval)
      ret = gptlstop ('automatic')
      ret = gptlstart ('alloc')
      call sub_alloc (npts, nz, chunksize, outval)
      ret = gptlstop ('alloc')
      ret = gptlstart ('stack')
      call sub_stack (npts, outval)
      ret = gptlstop ('stack')
    end do
  end do

  ret = gptlpr (0)
end program main
