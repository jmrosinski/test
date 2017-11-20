program driver
  implicit none
!$acc routine (sub2) seq
#ifdef CINTFC
  call ccode
#else
!$acc routine (sub1) seq
!$acc kernels
  call sub1
!$acc end kernels
#endif

  write(6,*)' back on cpu'
!$acc kernels
  call sub2
!$acc end kernels
end program driver
