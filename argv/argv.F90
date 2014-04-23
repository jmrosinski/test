program args
  implicit none

  integer :: nargs
  integer :: n
  character(len=64) :: arg

  integer, external :: command_argument_count
  external :: get_command_argument
  
  nargs = command_argument_count ()
  do n=1,nargs
    call get_command_argument (n, arg)
    write(6,*)'arg=',n,' value=', trim(arg)
  end do
end program args
