subroutine subimp (arr)
  implicit none

  real, intent(inout) :: arr(:)

  write(6,*)'subimp: size(arr)=', size(arr,1)
  write(6,*)'subimp: bounds(arr)=', lbound(arr,1), ubound(arr,1)
  return
end subroutine subimp
