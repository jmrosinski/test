program main
  implicit none

  character(len=2) :: archvtimeunit = 'zz'
  integer :: someint = 0

  namelist /mynl/ archvtimeunit, someint
  open (unit=1, file='mynl', form='formatted')
  read (1, mynl)
  write(6,*)'After reading namelist, archvtimeunit=', archvtimeunit
  write(6,*)'After reading namelist, someint=', someint
  stop
end program main

