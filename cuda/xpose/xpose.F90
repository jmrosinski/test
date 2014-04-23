program xpose
! Test program transposes an array on the GPU and sees that it's correct
  implicit none

  integer, parameter :: nip = 10242
  integer, parameter :: nz = 192

  integer :: ipn, k

  real :: arrki (nz,nip)
  real :: arrik (nip,nz)

  arrik(:,:) = -1.

  do ipn=1,nip
    do k=1,nz
      arrki(k,ipn) = ipn + k
    end do
  end do

  call sub (arrki, arrik, nip, nz)

! Verify correctness of results
  do ipn=1,nip
    do k=1,nz
      if (arrki(k,ipn) /= arrik(ipn,k)) then
        write(6,*) 'k,ipn,arrki,arrik=', k, ipn, arrki(k,ipn), arrik(ipn,k)
        stop 999
      end if
    end do
  end do
  write(6,*)'Transpose correct!'
  
  stop
end program xpose
