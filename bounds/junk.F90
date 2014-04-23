module junk
  implicit none

contains
  subroutine sub (arr)
    real, intent(in) :: arr(:,:)
    
    write(6,*) 'sub: lower bound of arr=', lbound(arr,1)
    write(6,*) 'sub: upper bound of arr=', ubound(arr,1)
  end subroutine sub
end module junk
