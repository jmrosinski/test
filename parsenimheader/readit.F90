program readit
  use module_control, only: nip, glvl, nz, yyyymmddhhmm

  implicit none

  character(len=4) :: varname
  character(len=14), parameter :: fn = 'out_wZZZ000001'
  integer :: its
  integer :: time
  integer :: dim1
  integer :: ios
  character(len=2) :: timeunit
  character(len=80) :: hdr(10)
  
  character*6 s6
  character*7 s7
  character*8 s8
  character*14 s14

  open(unit=1, file=fn, form='unformatted', status='old', iostat=ios)
  if (ios == 0) then
    write(6,*)'Successfully opened fn=', fn, ' for reading'
  else
    write(6,*)'Failed to open fn=', fn, ' for reading'
    stop 999
  end if
  read(1) hdr
  close(1)

  read(hdr(1),100) varname, yyyymmddhhmm
  100 format(4x,a4,t46,a12)

  read(hdr(2),201) nz, glvl, timeunit, its
  201 format(6x,i,8x,i,14x,a2,7x,i)

!  read(hdr(2),202) nz,glvl,timeunit,its,time
!  202 format(6x,i,8x,i,14x,a2,7x,i,2x,i)
  write(6,*)'nz,glvl,timeunit,its,time=',nz,glvl,timeunit,its,time
!  read(hdr(2),200) nz,glvl,timeunit,its,time
!  200 format(6x,i,7x,i,13x,a2,6x,i,1x,i)

  read(hdr(3),300) dim1,nip
  300 format(5x,i,6x,i)

  write(6,*)'varname=',varname
  write(6,*)'yyyymmddhhmm=',yyyymmddhhmm

  write(6,*)'nz=',nz
  write(6,*)'glvl=',glvl
  write(6,*)'timeunit=',timeunit
  write(6,*)'its=',its
  write(6,*)'time=',time

  write(6,*)'dim1=',dim1
  write(6,*)'nip=',nip
  stop 0
end program readit
