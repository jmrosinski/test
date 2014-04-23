module junk
  implicit none

contains
  subroutine sub (arr)
    real, intent(in) :: arr(:,:)
    
    write(6,*) 'dim1 of arr=', size(arr,1)
    write(6,*) 'dim2 of arr=', size(arr,2)
  end subroutine sub
end module junk
