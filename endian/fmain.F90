program main
  implicit none

  integer :: ioerr
  real :: arr(9)

  arr(:) = 9.
  open (unit=1, file='bigend', convert='big_endian', form='unformatted', status='replace', iostat=ioerr)
  if (ioerr /= 0) then
    write(6,*) 'Error opening bigend for writing'
    stop 999
  end if
  write(1) arr
  close (1)

  open (unit=1, file='littleend', convert='little_endian', form='unformatted', status='replace', iostat=ioerr)
  if (ioerr /= 0) then
    write(6,*) 'Error opening littleend for writing'
    stop 999
  end if
  write(1) arr
  close (1)

  stop 0
end program main
