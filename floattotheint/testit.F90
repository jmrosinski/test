program testit
  implicit none

  real :: base
  real :: result1, result2, result3
  integer :: intexp
  real :: floatexp

  do while (.true.)
    write(6,*)'Enter exponent:'
    read (5,*) intexp

    result1 = 2.**intexp
    floatexp = intexp
    result2 = 2.**floatexp
    result3 = 2**intexp

    write(6,*)'2.**intexp=',result1
    write(6,*)'2.**floatexp=',result2
    write(6,*)'2**intexp=',result3
  end do
end program testit
