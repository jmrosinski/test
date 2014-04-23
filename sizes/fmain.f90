program fmain
  implicit none

  integer iarr(2)
  real rarr(2)

  call csizes ()

  call fsizes (iarr(1), iarr(2), 'fortran integer size')
  call fsizes (rarr(1), rarr(2), 'fortran real size   ')
  stop
end program fmain
