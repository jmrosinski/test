program prlaheybits
  implicit none

  real(4) :: val4
  real(8) :: val8

  write(6,*)'32-bit lahey:'
  call prbits4 (val4)
  write(6,*)'64-bit lahey:'
  call prbits8 (val8)

  stop
end program prlaheybits
