program fpetest
  implicit none
  
  real :: giant = 1.e36
  real :: tiny = 1.e-36
  real :: x
  integer :: val

  real, external :: divide

  do while (.true.)
    write(6,*)'Enter 1 for zero divide, 2 for overflow, 3 for underflow, 4 for invalid:'
    read (5,*)val
    if (val == 1) then
      x = divide (1., 0.)
      write(6,*)'x=',x
    else if (val == 2) then
      x = divide (giant, tiny)
      write(6,*)'x=',x
    else if (val == 3) then
      x = divide (tiny, giant)
      write(6,*)'x=',x
    else if (val == 4) then
      x = sqrt(-giant)
      write(6,*)'x=',x
    end if
  end do
end program fpetest

real function divide (x, y)
  implicit none
  real, intent(in) :: x
  real, intent(in) :: y
  divide = x/y
  return
end function divide
