subroutine sub (n, testit)
  use defs
  implicit none

  integer, intent(in) :: n
  type (tobecopied), intent(inout) :: testit
  
  write(6,*)'Inside sub n=',n
  write(6,*)'Inside sub testit%x(n)=', testit%x(n)
  write(6,*)'Inside sub testit%y(n)%z=', testit%y(n)%z
end subroutine sub
