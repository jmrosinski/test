module dffusn
  implicit none

  private
  public :: dffusn_lev, dffusn_lyr

  interface dffusn_lev
    module procedure dffusn4_lev, dffusn8_lev
  end interface

  interface dffusn_lyr
    module procedure dffusn4_lyr, dffusn8_lyr
  end interface
  
contains

  subroutine dffusn4_lev (var)
    real*4, intent(in) :: var
    write(6,*)'Inside dffusn4_lev'
    return
  end subroutine dffusn4_lev

  subroutine dffusn8_lev (var)
    real*8, intent(in) :: var
    write(6,*)'Inside dffusn8_lev'
    return
  end subroutine dffusn8_lev

  subroutine dffusn4_lyr (var)
    real*4, intent(in) :: var
    write(6,*)'Inside dffusn4_lyr'
    return
  end subroutine dffusn4_lyr

  subroutine dffusn8_lyr (var)
    real*8, intent(in) :: var
    write(6,*)'Inside dffusn8_lyr'
    return
  end subroutine dffusn8_lyr
end module dffusn
