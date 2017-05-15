subroutine sub (n, testit)
  use defs
  implicit none

  integer, intent(in) :: n
  type (tobecopied), intent(inout) :: testit
  
  write(6,*)'Inside sub testit%x(9)=', testit%x(9)
end subroutine sub
