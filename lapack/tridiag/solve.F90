module solve

  private
  public :: tridiagsolve

#include <gptl.inc>

  contains

  subroutine tridiagsolve (n, iter)
    implicit none

    integer, intent(in) :: n     ! order of matrix
    integer, intent(in) :: iter  ! iteration count (> 1 for timing)

! LAPACK vars

    real :: dl(n-1)       ! lower diagonal
    real :: dlsave(n-1)   ! lower diagonal
    real :: d(n)          ! diagonal
    real :: dsave(n)      ! diagonal
    real :: du(n-1)       ! upper diagonal
    real :: dusave(n-1)   ! upper diagonal
    real :: du2(n-2)      ! output from sgttrf
    real :: b(n)          ! input: rhs. output: solution vector
                          ! Also: Diagonal elements for TRDSLV0
    real :: bsave(n)      ! rhs
    integer :: ipiv(n)    ! pivot elements (output from sgttrf)
    integer :: info       ! output from lapack routines

! TRDSLV0 vars

    real :: a(n)          ! sub-diagonal
    real :: c(n)          ! super-diagonal
    real :: x(n)          ! X from AX = B (solution vector)
    real :: y(n)          ! B from AX = B (RHS)

! Other local vars

    integer :: i
    integer :: ret
  
    ! Initialize A and B of AX = B
    do i=1,n-1
      dl(i) = i
      du(i) = i + 2
    end do
    
    do i=1,n
      d(i) = i + 1
      b(i) = 1 - i
    end do

    dlsave(:) = dl(:)
    dsave(:)  = d(:)
    dusave(:) = du(:)
    bsave(:)  = b(:)
    
    ret = gptlstart ('LAPACK')
    call sgttrf (n, dl, d, du, du2, ipiv, info)
    if (info /= 0) then
      write(6,*)'Bad info returned by sgttrf=', info
      stop 999
    end if
    call sgttrs ('N', n, 1, dl, d, du, du2, ipiv, b, n, info)
    if (info /= 0) then
      write(6,*)'Bad info returned by sgttrs=', info
      stop 999
    end if
    ret = gptlstop ('LAPACK')
    
    if (iter == 1) then
      call verify_result (n, dlsave, dsave, dusave, bsave, b)
    end if

    ! Now use default NIM solver

    a(1)     = 1000000
    a(2:n)   = dlsave(1:n-1)
    b(:)     = dsave(:)
    c(1:n-1) = dusave(1:n-1)
    c(n)     = dusave(n)
    y(:)     = bsave(:)

    ret = gptlstart ('TRDSLV0')
    call trdslv0 (n, a, b, c, y, x)
    ret = gptlstop ('TRDSLV0')

    if (iter == 1) then
      call verify_result (n, dlsave, dsave, dusave, bsave, x)
    end if

    return
  end subroutine tridiagsolve
  
  subroutine verify_result (n, dl, d, du, b, x)
    implicit none
    
    integer, intent(in) :: n
    real, intent(in) :: dl(:)
    real, intent(in) :: d(:)
    real, intent(in) :: du(:)
    real, intent(in) :: b(:)
    real, intent(in) :: x(:)
    
    integer :: i, ii
    real :: result
    real :: abserr
    real :: relerr
    
    i = 1
    result = d(i)*x(i) + du(i)*x(i+1)
    abserr = result - b(i)
    relerr = abs (abserr / b(i))
    write(6,*)'abserr(',i,') = ',abserr
    write(6,*)'relerr(',i,') = ',relerr
    
    do i=2,n-1
      result = dl(i-1)*x(i-1) + d(i)*x(i) + du(i)*x(i+1)
      abserr = result - b(i)
      relerr = abs (abserr / b(i))
      write(6,*)'abserr(',i,') = ',abserr
      write(6,*)'relerr(',i,') = ',relerr
    end do
    
    i = n
    result = dl(i-1)*x(i-1) + d(i)*x(i)
    abserr = result - b(i)
    relerr = abs (abserr / b(i))
    write(6,*)'abserr(',i,') = ',abserr
    write(6,*)'relerr(',i,') = ',relerr
  end subroutine verify_result
end module solve
