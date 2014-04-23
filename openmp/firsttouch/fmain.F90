program main
  use gptl

  implicit none

  integer, parameter :: nip = 102420
  integer, parameter :: nvl = 64
#ifdef ALLOC
  real, allocatable :: arr1(:,:)
  real, allocatable :: arr2(:,:)
  real, allocatable :: sum1(:)
  real, allocatable :: sum2(:)
#else
  real :: arr1(nvl,nip)
  real :: arr2(nvl,nip)
  real :: sum1(nip)
  real :: sum2(nip)
#endif
  real(8) :: value
  integer :: ipn, k, n
  integer :: ret

#ifdef ALLOC
  allocate(arr1(nvl,nip))
  allocate(sum1(nip))
  allocate(arr2(nvl,nip))
  allocate(sum2(nip))
#endif

  ret = gptlsetutr (gptlnanotime)
  ret = gptlinitialize ()
  ret = gptlstart ('total')
!$OMP PARALLEL DO PRIVATE(k)
  do ipn=1,nip
    sum1(ipn) = 0.
    do k=1,nvl
      arr1(k,ipn) = 1.
    end do
  end do
!$OMP END PARALLEL DO

  do ipn=1,nip
    sum2(ipn) = 0.
    do k=1,nvl
      arr2(k,ipn) = 1.
    end do
  end do
  
  ret = gptlstart ('sub1')
  do n=1,1000
    call sub(arr1, sum1, nvl, nip)
  end do
  ret = gptlstop ('sub1')

  ret = gptlstart ('sub2')
  do n=1,1000
    call sub(arr2, sum2, nvl, nip)
  end do
  ret = gptlstop ('sub2')

  ret = gptlget_wallclock ('sub1', 0, value)
  write(6,*) 'Time for first touch=', value
  ret = gptlget_wallclock ('sub2', 0, value)
  write(6,*) 'Time for no first touch=', value

  stop
end program main
