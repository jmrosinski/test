subroutine sub2 (x, var, nx, ny, nz)
  use gptl
  implicit none

  integer, intent(in) :: nx, ny, nz
  real, intent(in) :: x(nx,ny,nz)
  real, intent(inout) :: var
  real :: y(nx,ny,nz)
  integer :: i,j,k
  integer :: ret

  ret = gptlprint_memusage ('in sub2 before setting y')
  y(:,:,:) = 0.
  ret = gptlprint_memusage ('in sub2 after setting y')
  do k=1,nz
    do j=1,ny
      do i=1,nx
        y(i,j,k) = x(i,j,k) + i + j + k
        var = var + y(i,j,k)
      end do
    end do
  end do

  return
end subroutine sub2
