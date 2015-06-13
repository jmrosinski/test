module problems
  implicit none
  real :: modarr(1)
  real, allocatable, save :: allocarr(:)
contains
  subroutine boundsread (arr)
    real, intent(inout) :: arr(:,:)
    real :: x

    write(6,*)'bounds reading...'
    x = arr(10,10)
  end subroutine boundsread

  subroutine boundswrite (arr)
    real, intent(inout) :: arr(:,:)
    real :: x

    write(6,*)'bounds writing...'
    arr(10,10) = 2.
  end subroutine boundswrite

  subroutine testintent (arr)
    real, intent(out) :: arr(:,:)
  end subroutine testintent
      
  subroutine uninit_stack (arr)
    real, intent(inout) :: arr(:,:)
    real :: x

    write(6,*)'uninit_stack setting...'
    arr(1,1) = x
    write(6,*)'uninit_stack setting with math...'
    arr(1,1) = x + 1.
  end subroutine uninit_stack

  subroutine uninit_static (arr)
    real, intent(inout) :: arr(:,:)
    real, save :: x

    write(6,*)'uninit_static setting...'
    arr(1,1) = x
    write(6,*)'uninit_static setting with math...'
    arr(1,1) = x + 1.
  end subroutine uninit_static

  subroutine uninit_mod (arr)
    real, intent(inout) :: arr(:,:)

    write(6,*)'uninit_mod setting...'
    arr(1,1) = modarr(1)
    write(6,*)'uninit_mod setting with math...'
    arr(1,1) = modarr(1) + 1.
  end subroutine uninit_mod

  subroutine uninit_alloc (arr)
    real, intent(inout) :: arr(:,:)

    allocate (allocarr(9))
    write(6,*)'uninit_alloc setting...'
    arr(1,1) = allocarr(1)
    write(6,*)'uninit_alloc setting with math...'
    arr(1,1) = allocarr(1) + 1.
    deallocate (allocarr)
  end subroutine uninit_alloc

  subroutine dangle (danglearr)
    real, intent(in) :: danglearr(:,:)
    real :: x
    write(6,*)'dangle: dereference dangling pointer...'
    x = danglearr(1,1)
    write(6,*)'dangle: print dereferenced dangling pointer...'
    write(6,*)'x=',x
  end subroutine dangle
end module problems
