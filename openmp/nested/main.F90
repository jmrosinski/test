program main
  implicit none

  integer :: i1, i2, i3, i4, i5, i6, i7
  integer :: mythread
  integer, external :: omp_get_thread_num
  integer :: i, n, j
  integer, parameter :: numsends=2
  integer :: NumToSend(numsends) = (/1,2/)
  integer, parameter :: totlen = 100
  integer :: i7s = 1
  integer :: i7e = 1
  integer :: i6s = 1
  integer :: i6e = 1
  integer :: i5s = 1
  integer :: i5e = 1
  integer :: i4s = 1
  integer :: i4e = 2
  integer :: i3e
  integer :: i2s = 1
  integer :: i2e = 4
  integer :: i1s = 1
  integer :: i1e = 5
  integer :: s1, s2, s4, s5, s6
  integer :: x(totlen)

  x(:) = 0.
  s1 = i1e-i1s+1
  s2 = i2e-i2s+1
  s4 = i4e-i4s+1
  s5 = i5e-i5s+1
  s6 = i6e-i6s+1
#ifdef OLDWAY
#else
  i3e = maxval (NumToSend(:))
!$OMP PARALLEL DO PRIVATE (n,i7,i6,i5,i4,i3,i2,j,i1,i) COLLAPSE (7)
#endif
  do n=1,numsends
#ifdef OLDWAY
    j = 0
    i3e = NumToSend(n)
#endif
    do i7=i7s,i7e
      do i6=i6s,i6e
        do i5=i5s,i5e
          do i4=i4s,i4e
#ifdef OLDWAY
!$OMP PARALLEL DO PRIVATE(i2,j,i1,i)
#endif
            do i3=1,i3e
              do i2=i2s,i2e
                if (i3 <= NumToSend(n)) then
#ifdef OLDWAY
#else
                  j = i4 - i4s + (i5-i5s)*s4*(1 + (i6-i6s)*s5*(1 + (i7-i7s)*s6))
#endif
                  do i1=i1s,i1e
                    i = (j*i3e+(i3-1))*s1*s2+(i2-i2s)*s1+(i1-i1s+1)
                    x(i) = n+i+j
                  end do
                end if
              end do
            end do
#ifdef OLDWAY
            j=j+1
#endif
          end do
        end do
      end do
    end do
  end do

  do i=1,totlen
    if (x(i) /= 0) then
      write(6,*)'x(',i,')=',x(i)
    end if
  end do

  stop
end program main
