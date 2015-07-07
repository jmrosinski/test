module subs
  implicit none

contains

  subroutine sub_automatic (chunksize, nz, npts, outval)
    integer, intent(in) :: chunksize, nz, npts
    real, intent(out) :: outval

    real :: arr(chunksize,nz)

    arr(:npts,:) = 0.
    outval = arr(chunksize,nz)
  end subroutine sub_automatic

  subroutine sub_alloc (chunksize, nz, npts, outval)
    integer, intent(in) :: chunksize, nz, npts
    real, intent(out) :: outval

    real, allocatable :: arr(:,:)

    allocate (arr(chunksize,nz))
    arr(:npts,:) = 0.
    outval = arr(npts,nz)
    deallocate (arr)
  end subroutine sub_alloc

  subroutine sub_stack (npts, outval)
    use parms, only: nz, chunksize
    integer, intent(in) :: npts
    real, intent(out) :: outval

    real :: arr(chunksize,nz)

    arr(:npts,:) = 0.
    outval = arr(npts,nz)
  end subroutine sub_stack
end module subs
