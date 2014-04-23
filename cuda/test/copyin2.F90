subroutine copyin2 (n, chunksize, nchunks, niter)
  implicit none

  integer, intent(in) :: n,chunksize,nchunks,niter
  real :: arrout2(n,chunksize,nchunks,niter)

!ACC$DATA(<arrout2:none>)

  return
end subroutine copyin2
