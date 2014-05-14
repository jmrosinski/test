program fmain
  implicit none

  logical :: linebuf = .false.
  character(len=1) :: ans

  write(6,*)'Enter y to line buffer stdout'
  read (5,*) ans
  if (ans == 'y') then
    linebuf = .true.
  end if

  if (linebuf) then
    call linebuf_stdout ()
  end if
  write(6,*)'Starting print with linebuf=', linebuf

  write(6,*) '1st line to stdout'
  write(0,*) '1st line to stderr comes after 1st line to stdout'
  write(6,*) '2nd line to stdout comes after 1st line to stderr'
  stop 0
end program fmain
