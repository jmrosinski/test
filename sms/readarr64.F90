subroutine readarr64 (arr, dim2siz, unitno)
  use decomp
  
  implicit none

  integer, intent(in) :: dim2siz  ! size of 2nd dimension of arr
  integer, intent(in) :: unitno   ! unit number to read from
    
!SMS$DISTRIBUTE(dh,1) BEGIN
  real*8, intent(inout) :: arr(nip,dim2siz)
!SMS$DISTRIBUTE END

! The <arr,out> keeps SMS from generating a needless gather before the read
!SMS$SERIAL (<ARR,OUT> : DEFAULT=IGNORE) BEGIN
  read (unitno, err=90) arr
  write(6,*)'readarr64: arr(1,1)=',arr(1,1)
!  write(6,*)'           arr(nip/4,dim2siz)=',arr(nip/4,dim2siz)
!  write(6,*)'           arr(nip/2,dim2siz)=',arr(nip/2,dim2siz)
!  write(6,*)'           arr(nip,dim2siz)=',arr(nip,dim2siz)
  write (6,100) arr
100 format(1p,10e10.1)
!SMS$SERIAL END
  return

90 write(6,*) 'readarr64: error reading from unit ', unitno, ' Stopping'
  stop
end subroutine readarr64
