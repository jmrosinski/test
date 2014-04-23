program writit
  implicit none

  real(8) :: arr(9)
  integer :: ierr, i
  character(len=*), parameter :: fname = 'ninexreal8'

  do i=1,9
    arr(i) = 1./i
  end do

  open (unit=1, form='unformatted', status='replace', file=fname, iostat=ierr)
  if (ierr /= 0) then
    write(6,*)'Error creating ',fname
    stop 999
  end if
  write(1)arr
  stop 0
end program writit
