program main
  use problems
  implicit none

  real, allocatable, target :: allarr(:,:)
  real, pointer :: parr(:,:)
  real :: arr(9,9)
  real :: x
  integer :: n
  integer :: ans
  character(len=16) :: funcs(8) = (/'boundsread      ', &
                                    'boundswrite     ', &
                                    'testintent      ', &
                                    'uninit_stack    ', &
                                    'uninit_static   ', &
                                    'uninit_mod      ', &
                                    'uninit_alloc    ', &
                                    'dangle          '/)

  do while (.true.)
    write(6,*)'Enter a number for a test:'
    do n=1,size(funcs)
      write(6,'(i2,1x,a16)')n,funcs(n)
    end do
    read(5,*)ans

    select case (ans)
    case (1)
      call boundsread (arr)
    case (2)
      call boundswrite (arr)
    case (3)
      arr(1,1) = 1.
      call testintent (arr)
      x = arr(1,1)
      write(6,*)'main after testintent: x=',x
    case (4)
      call uninit_stack (arr)
    case (5)
      call uninit_static (arr)
    case (6)
      call uninit_mod (arr)
    case (7)
      call uninit_alloc (arr)
    case (8)
      allocate (allarr(9,9))
      allarr(:,:) = 7.
      parr => allarr(:,:)
      deallocate (allarr)
      call dangle (parr)
      nullify (parr)
    case default
      write(6,*)'Case ', ans, ' not found'
    end select
  end do
end program main
