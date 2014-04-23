program main
  use dffusn, only: dffusn_lev, dffusn_lyr

  implicit none

  real*4 :: var4 = 0.
  real*8 :: var8 = 0.

  write(6,*)'calling dffusn_lev with var4'
  call dffusn_lev(var4)
  write(6,*)'calling dffusn_lev with var8'
  call dffusn_lev(var8)
  write(6,*)'calling dffusn_lyr with var4'
  call dffusn_lyr(var4)
  write(6,*)'calling dffusn_lyr with var8'
  call dffusn_lyr(var8)

  stop
end program main
