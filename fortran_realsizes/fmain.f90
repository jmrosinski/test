program main
  implicit none

  integer, parameter :: r6 = selected_real_kind(6)
  integer, parameter :: r7 = selected_real_kind(7)
  integer, parameter :: r8 = selected_real_kind(8)
  integer, parameter :: r14 = selected_real_kind(14)
  integer, parameter :: r15 = selected_real_kind(15)
  integer, parameter :: r16 = selected_real_kind(16)
  integer, parameter :: r17 = selected_real_kind(17)

  real(r6) :: x6(2)
  real(r7) :: x7(2)
  real(r8) :: x8(2)
  real(r14) :: x14(2)
  real(r15) :: x15(2)
  real(r16) :: x16(2)
  real(r17) :: x17(2)

  call printdist ('r6'//char(0), x6(1), x6(2))
  call printdist ('r7'//char(0), x7(1), x7(2))
  call printdist ('r8'//char(0), x8(1), x8(2))
  call printdist ('r14'//char(0), x14(1), x14(2))
  call printdist ('r15'//char(0), x15(1), x15(2))
  call printdist ('r16'//char(0), x16(1), x16(2))
  call printdist ('r17'//char(0), x17(1), x17(2))
end program main
