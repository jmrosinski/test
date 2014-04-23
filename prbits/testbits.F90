program testbits
  implicit none

! z=hex b=binary
!  real(4) :: val = z'7f800000'   ! inf
  real(4) :: val = z'ffc00000'    ! nan

  real(4) :: inf = b'01111111100000000000000000000000'
  real(4) :: nan = b'11111111110000000000000000000000'
  real(4) :: lahey = b'10001011100010111000101110001011'

  write(6,*)'inf=', inf
  write(6,*)'nan=', nan
  write(6,*)'val=', val
  write(6,*)'lahey=', lahey
  stop
end program testbits
