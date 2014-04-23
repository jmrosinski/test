program fmain
  implicit none

  real(4) :: num4
  real(4) :: den4
  real(4) :: val4
  real(8) :: num8
  real(8) :: den8
  real(8) :: val8

  integer :: bpw   ! bytes per word

  do while (.true.)
    write(6,*)'Enter 4 for 4-byte reals, 8 for 8-byte reals'
    read (5,*) bpw
    if (bpw /= 4 .and. bpw /= 8) then
      cycle
    end if

    if (bpw == 4) then
      write(6,*)'Enter 32-bit numerator:'
      read (5,*) num4
      write(6,*)'Enter 32-bit denominator:'
      read (5,*) den4

      val4 = num4 / den4
      write(6,*)'Calling prbits4 for:', val4
      call prbits4 (val4)
    else    ! bpw == 8
      write(6,*)'Enter 64-bit numerator:'
      read (5,*) num8
      write(6,*)'Enter 64-bit denominator:'
      read (5,*) den8

      val8 = num8 / den8
      write(6,*)'Calling prbits8 for:', val8
      call prbits8 (val8)
    end if
  end do
  stop
end program fmain
