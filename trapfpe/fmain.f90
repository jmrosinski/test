program fmain
  implicit none

  real :: z
  real :: one = 1.
  real :: zero = 0.
  real :: small = 1.e-30
  real :: big = 1.e+30
  integer :: ans
  character(len=1) :: lang
  real, external :: fortfunc, cfunc

  do while (.true.)
    write(6,*)"Enter 0 for divide by zero, 1 for overflow, 2 for underflow, 3 for invalid:"
    read (5,*) ans

    write(6,*)"Enter f for Fortran computational routine, c for C:"
    read (5,*) lang

    if (lang == 'c') then
      select case (ans)
      case (0)
        write(6,*) "Calling cfunc to divide by zero (1./0.):"
        z = cfunc (one, zero)
        write(6,*)'result=',z
      case (1)
        write(6,*) "Calling cfunc to overflow (big/small):"
        z = cfunc (big, small)
        write(6,*)'result=',z
      case (2)
        write(6,*) "Calling cfunc to underflow (small/big):"
        z = cfunc (small, big)
        write(6,*)'result=',z
      case (3)
        write(6,*) "Calling cfunc to issue invalid instruction (0./0.):"
        z = cfunc (zero, zero)
        write(6,*)'result=',z
      case default
        write(6,*) "You must enter 0, 1, 2, or 3: ", ans, " is not valid"
      end select
    else if (lang == 'f') then
      select case (ans)
      case (0)
        write(6,*) "Calling fortfunc to divide by zero (1./0.):"
        z = fortfunc (one, zero)
        write(6,*)'result=',z
      case (1)
        write(6,*) "Calling fortfunc to overflow (big/small):"
        z = fortfunc (big, small)
        write(6,*)'result=',z
      case (2)
        write(6,*) "Calling fortfunc to underflow (small/big):"
        z = fortfunc (small, big)
        write(6,*)'result=',z
      case (3)
        write(6,*) "Calling fortfunc to issue invalid instruction (0./0.):"
        z = fortfunc (zero, zero)
        write(6,*)'result=',z
      case default
        write(6,*) "You must enter 0, 1, 2, or 3: ", ans, " is not valid"
      end select
    end if
  end do
end program fmain
