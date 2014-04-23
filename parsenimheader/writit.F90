program writit
  use module_control, only: nip, glvl, nz, yyyymmddhhmm
  use module_header

  implicit none

  character(len=4), parameter :: varname = 'wZZZ'  ! hardwire
  character(len=14) :: fn
  character(len=2), parameter :: timeunit = 'ts'
  integer :: its = 1
  integer :: time
  integer :: ios

  write(6,*)'Enter glvl'
  read(5,*) glvl
  write(6,*)'Enter nz'
  read(5,*) nz

  time = its
  yyyymmddhhmm = '201311150101'  ! hardwire
  nip = 10*(2**glvl)**2 + 2
  write(fn,'("out_",a4,i6.6)') varname, its
  write(6,*)'glvl=', glvl,' nz=', nz, ' its=', its, ' nip=',nip

  open(unit=1, file=fn, form='unformatted', status='replace', iostat=ios)
  if (ios == 0) then
    write(6,*)'Successfully opened fn=', fn, ' for writing'
  else
    write(6,*)'Failed to open fn=', fn, ' for writing'
    stop 999
  end if
  write(1) header (varname, nz, its, time, timeunit)
  close(1)

  write(6,*)'Done writing fn=', fn
  stop 0
end program writit
