module subs
  implicit none

contains

  subroutine sub (n, arr)
    integer, intent(in) :: n
    real, intent(inout) :: arr(:)

    return
  end subroutine sub
end module subs
