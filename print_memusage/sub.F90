subroutine sub (var, nx, ny, nz)
  use gptl
  implicit none

  integer, intent(in) :: nx, ny, nz
  real, intent(inout) :: var
  real :: x(nx,ny,nz)
  integer :: i,j,k
  integer :: ret

  ret = gptlprint_memusage ('in sub before setting x')
  x(:,:,:) = 0.
  ret = gptlprint_memusage ('in sub after setting x')
  do k=1,nz
    do j=1,ny
      do i=1,nx
        x(i,j,k) = x(i,j,k) + i + j + k
        var = var + x(i,j,k)
      end do
    end do
  end do
  call sub2 (x, var, nx, ny, nz)

  return
end subroutine sub
