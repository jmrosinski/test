program main
  implicit none

  integer, parameter :: negint = -999
  integer :: n
  integer :: nthreads
  integer :: levs 
  integer :: numleft
  integer, allocatable :: kbeg(:)
  integer, allocatable :: numk(:)

  do while (.true.)
    write(6,*)'Enter nthreads'
    read (5,*)nthreads
    if (nthreads < 1) stop 0
    write(6,*)'Enter levs'
    read (5,*)levs
    if (levs < 1) stop 0

    allocate (kbeg(nthreads))
    allocate (numk(nthreads))

    if (nthreads >= levs) then              ! 1 k per thread until threads exhausted
      do n=1,levs
        kbeg(n) = n
        numk(n) = 1
      end do
      kbeg(levs+1:nthreads) = negint
      numk(levs+1:nthreads) = negint
    else if (mod (levs, nthreads) == 0) then ! levs > nthreads and divides evenly into levs
      kbeg(1) = 1
      numk(1) = levs / nthreads
      do n=2,nthreads
        kbeg(n) = kbeg(n-1) + numk(n-1)
        numk(n) = numk(n-1)
      end do
    else                                          ! levs > nthreads
      kbeg(1) = 1
      numk(1) = levs / nthreads + 1
      do n=2,nthreads
        kbeg(n) = kbeg(n-1) + numk(n-1)
        numleft = levs - kbeg(n) + 1
        if (mod (numleft, nthreads-n+1) == 0) then
          numk(n) = numleft / (nthreads-n+1)
        else
          numk(n) = numleft / (nthreads-n+1) + 1
        end if
      end do
    end if

    write(6,100)nthreads, levs
100 format ('nthreads=',i3,' levs=',i3)
    write(6,101) 'kbeg=',kbeg(:)
    write(6,101) 'numk=',numk(:)
101 format (a,64i4)
    deallocate (kbeg)
    deallocate (numk)
  end do
end program main
