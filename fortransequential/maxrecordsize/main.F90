program main
  implicit none

  real, allocatable :: arr(:,:,:)
  integer, parameter :: nx = 1000
  integer, parameter :: ny = 1000
  integer, parameter :: nz = 500
  integer :: i, j, k
  character(len=4) :: str

  allocate (arr(nx,ny,nz))

  do k=1,nz
    do j=1,ny
      do i=1,nx
        arr(i,j,k) = i+j+k
      end do
    end do
  end do

  write(6,*)'opening for write...'
  open (unit=1, file='testout', form='unformatted', err=10)
  write(6,*)'writing...'
  write(1,err=20) arr
!  write(1,err=20) 'done'
  close (unit=1)

  arr(:,:,:) = 0.

  write(6,*)'opening for read...'
  open (unit=1, file='testout', form='unformatted', action='read', err=30)
  write(6,*)'reading...'
  read(1,err=40) arr
!  read(1,err=20) str
  close (unit=1)

  write(6,*)'checking...'
  do k=1,nz
    do j=1,ny
      do i=1,nx
        if (arr(i,j,k) /= i+j+k) then
          write(6,*)'Error at i,j,k=', i,j,k
          write(6,*)'arr=', arr(i,j,k)
          stop 999
        end if
      end do
    end do
  end do

!  if (str /= 'done') then
!    write(6,*)'Error in str=', str
!    stop 999
!  end if

  stop 0
  
10 write(6,*)'Error opening testout for write'
  stop 999

20 write(6,*)'Error writing testout'
  stop 999

30 write(6,*)'Error opening testout for read'
  stop 999

40 write(6,*)'Error reading testout'
  stop 999
end program main

