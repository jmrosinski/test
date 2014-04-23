module infnan
!SMS$IGNORE BEGIN
  implicit none

  private
  public :: inf, nan, negint, isinf, isnan

! TODO: add big endian ifdef for e.g. IBM
! TODO: Should NaN be signaling or non-signaling?

  real,    parameter :: inf = z'7f800000'   ! Infinity
  real,    parameter :: nan = z'ffc00000'   ! NaN
  integer, parameter :: negint = -999       ! Bad integer value

contains

! These functions are experimental! If they work they can provide a nice means to test
! if something has become an inf or a nan

  logical function isnan (x)
    real :: x

    isnan = (x < 0. .eqv. x >= 0.)
    return
  end function isnan

  logical function isinf (x)
    real :: x

    isinf = abs(x) > huge (x)
    return
  end function isinf
!SMS$IGNORE END
end module infnan
