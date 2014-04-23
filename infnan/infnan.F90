program test
  implicit none

! TODO: add big endian ifdef for e.g. IBM
  real,    parameter :: inf = z'7f800000'   ! Infinity
  real,    parameter :: nan = z'ffc00000'   ! NaN
  integer, parameter :: negint = -999
  logical :: x

  write(6,*)'inf=',inf
  write(6,*)'nan=',nan

  if (nan < 0. .eqv. nan >= 0.) then
    write(6,*) 'nan test works'
  else
    write(6,*) 'nan test failed'
  end if

  if (abs(inf) > huge(inf)) then
    write(6,*) 'inf test works'
  else
    write(6,*) 'inf test failed'
  end if

end program test
