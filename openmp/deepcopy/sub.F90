subroutine sub (n, testit)
  use defs
  implicit none

  integer, intent(in) :: n
  type (tobecopied), intent(inout) :: testit
  integer(8), external :: printaddr
  
  write(6,100)'Inside sub n=',n,' Address of x=',printaddr(testit%x(n))
100 format(a,i2,a,z16)
  write(6,100)'Inside sub n=',n,' Address of z=',printaddr(testit%y(n)%z)
  write(6,*)'Inside sub testit%x(n)=', testit%x(n)
  write(6,*)'Inside sub testit%y(n)%z=', testit%y(n)%z
end subroutine sub
