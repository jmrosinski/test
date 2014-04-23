subroutine sub (arrki, arrik, nip, nz)
  implicit none

  integer, intent(in) :: nip, nz
  real, intent(in) :: arrki(nz,nip)
  real, intent(out) :: arrik(nip,nz)

  integer :: ipn, k

! Transfer the data to the GPU
!!!!ACC$DATA(<arrki,arrik:in>)

! Do the transpose on the GPU

!ACC$REGION(<nz>,<nip>,<arrki:in>,<arrik:out>) BEGIN
!ACC$DO PARALLEL (1)
  do ipn=1,nip
!ACC$DO VECTOR (1)
    do k=1,nz
      arrik(ipn,k) = arrki(k,ipn)
    end do
  end do
!ACC$REGION END

! Transfer the transposed data back to the CPU
!!!!ACC$DATA(<arrki,arrik:out>)

  return
end subroutine sub

