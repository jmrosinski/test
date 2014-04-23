subroutine copyout2 (arrout2, n, chunksize, nchunks, niter)
  implicit none

  integer, intent(in) :: n, chunksize, nchunks, niter
  real, intent(inout) :: arrout2(n,chunksize,nchunks,niter)

!ACC$DATA(<arrout2:out>)

  return
end subroutine copyout2
