program driver
  implicit none

  integer :: iargc

  character(len=128) :: arg
  integer :: numargs
  integer :: i

  numargs = iargc()
  write(6,*)'Found ', numargs, ' cmd-line args'
  do i=1,numargs
    call getarg (i, arg)
    write(6,*)'arg number ', i, ' is ', trim(arg)
  end do
  stop
end program driver
