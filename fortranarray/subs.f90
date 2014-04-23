module subs
  implicit none

CONTAINS

  subroutine submod (arr)
    real, intent(inout) :: arr(:)

    write(6,*)'subinmod: size(arr)=', size(arr,1)
    write(6,*)'subinmod: bounds(arr)=', lbound(arr,1), ubound(arr,1)
    return
  end subroutine submod
end module subs

    
