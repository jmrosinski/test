subroutine writearr64 (arr, dim2siz, unitno)
  use decomp
  
  implicit none

  integer, intent(in) :: dim2siz  ! size of 2nd dimension of arr
  integer, intent(in) :: unitno   ! unit number to write to
    
!SMS$DISTRIBUTE(dh,1) BEGIN
  real*8, intent(in) :: arr(nip,dim2siz)
!SMS$DISTRIBUTE END

! The <arr,in> keeps SMS from generating a needless scatter after the write
!SMS$SERIAL (<ARR,IN> : DEFAULT=IGNORE) BEGIN
  write (unitno, err=90) arr
  write(6,*)'writearr64: arr(1,1)=',arr(1,1)
!  write(6,*)'           arr(nip/4,dim2siz)=',arr(nip/4,dim2siz)
!  write(6,*)'           arr(nip/2,dim2siz)=',arr(nip/2,dim2siz)
!  write(6,*)'           arr(nip,dim2siz)=',arr(nip,dim2siz)
  write (6,100) arr
100 format(1p,10e10.1)
!SMS$SERIAL END
  return

90 write(6,*) 'writearr64: error writing to unit ', unitno, ' Stopping'
  stop
end subroutine writearr64
