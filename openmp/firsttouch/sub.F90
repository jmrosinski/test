subroutine sub (arr, sum, nvl, nip)
  implicit none

  integer, intent(in) :: nvl, nip
  real, intent(in) :: arr(nvl,nip)
  real, intent(inout) :: sum(nip)

  integer k, ipn

!$OMP PARALLEL DO PRIVATE(k)
  do ipn=1,nip
    do k=1,nvl
      sum(ipn) = sum(ipn) + 0.1*arr(k,ipn)
    end do
  end do

  return
end subroutine sub
