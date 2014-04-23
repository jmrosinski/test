program junk
  implicit none

  integer, parameter :: nz=32
  integer, parameter :: nip=3
  integer, parameter :: pad=2

  real :: arr(nz,nip+pad)
  integer :: ipn,k

  arr(:,:) = -1.

  do ipn=1,nip
    do k=1,nz
      arr(k,ipn) = k + (ipn-1)*nz
    end do
  end do

  write(6,*)'arrmax=', maxval(arr(:,1:nip))
  write(6,*)'arrmin=', minval(arr(:,1:nip))
  
end program junk
