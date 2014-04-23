program memrefs
  implicit none
  
#include <gptl.inc>

  integer, parameter :: nelem = 1024000
  integer :: ret
  integer :: code
  integer :: i, j, ind

  real(4) :: arr4(nelem)
  real(4) :: sum4 = 0.
  real(8) :: arr8(nelem)
  real(8) :: sum8 = 0.

  ret = gptlevent_name_to_code ('PAPI_L1_DCA', code)
  if (ret /= 0) then
    write(6,*)'Error in gptleventname_to_code: ret=', ret
    stop 999
  end if

  ret = gptlsetoption (code, 1)
  if (ret /= 0) then
    write(6,*)'Error in gptlsetoption: ret=', ret
    stop 999
  end if

  ret = gptlinitialize ()
  arr4(:) = 0.1
  arr8(:) = 0.2

  ret = gptlstart ('real4_stride1')
  do i=1,nelem
    sum4 = sum4 + arr4(i)
  end do
  ret = gptlstop ('real4_stride1')

  ret = gptlstart ('real8_stride1')
  do i=1,nelem
    sum8 = sum8 + arr8(i)
  end do
  ret = gptlstop ('real8_stride1')

  ret = gptlstart ('real8_stride8')
  do j=1,8
    do i=1,nelem,8
      ind = min (i+j, nelem)
      sum8 = sum8 + arr8(ind)
    end do
  end do
  ret = gptlstop ('real8_stride8')
  ret = gptlpr(0)

  write(6,*)'sum=', sum4, sum8
  stop
end program memrefs
