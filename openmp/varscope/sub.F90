subroutine sub (iter)
  use zzz
  implicit none

  integer, intent(in) :: iter
  real :: var1
  real :: var2(1)

  character(len=8) :: str

  write(str,'(a5,i3.3)') 'iter=',iter

  write(6,*)'calling printmyaddress iter=',iter
  call printmyaddress (str//'var1', var1)
  call printmyaddress (str//'var2', var2(1))
  return
end subroutine sub
