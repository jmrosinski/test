program main
  use gptl
  implicit none

  integer :: testtablesize = 1
  integer :: ret

  call gptlprocess_namelist ('namelist', 77, ret, 77)
  if (ret == 0) then
    write(6,*)'Successfully processed namelist'
  else
    write(6,*)'Failure processing namelist: ret=', ret
  end if
  stop
end program main
