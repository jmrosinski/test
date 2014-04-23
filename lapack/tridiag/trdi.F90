!TBH:  KS=0 in all of our calls so removed WORK from interface.  
!TBH:  Also updated to use INTENT, replaced GOTOs and older syntax.  

!
!     SUBROUTINE TRDI (N,A,B,C,Y,X,KS,WORK)
!
!
! DIMENSION OF           A(N), B(N), C(N), Y(N), X(N), WORK(2*N-2)
! ARGUMENTS
!
! LATEST REVISION        MARCH 1985
!
! PURPOSE                TRDI COMPUTES THE SOLUTION OF THE TRIDIAGONAL
!                        LINEAR SYSTEM,
!                            B(1)*X(1)+C(1)*X(2)               = Y(1)
!                            A(I)*X(I-1)+B(I)*X(I)+C(I)*X(I+1) = Y(I)
!                                I=2,3,...,N-1
!                            A(N)*X(N-1)+B(N)*X(N)             = Y(N)
!
! USAGE                  CALL TRDI (N,A,B,C,Y,X,KS,WORK)
!
! ARGUMENTS
!
! ON INPUT               N
!                          THE NUMBER OF UNKNOWNS.  THIS SUBROUTINE
!                          REQUIRES THAT  N  BE GREATER THAN  2.
!
!                        A
!                          THE SUBDIAGONAL OF THE MATRIX IS STORED IN
!                          LOCATIONS A(2) THROUGH A(N).
!
!                        B
!                          THE DIAGONAL OF THE MATRIX IS STORED IN
!                          LOCATIONS B(1) THROUGH B(N).
!
!                        C
!                          THE SUPER-DIAGONAL OF THE MATRIX IS STORED IN
!                          LOCATIONS C(1) THROUGH C(N-1).
!
!                        Y
!                          THE RIGHT-HAND SIDE OF THE EQUATIONS IS
!                          STORED IN Y(1) THROUGH Y(N).
!
!                        KS
!                          = 0  IF THIS IS THE INITIAL CALL TO TRDI WITH
!                               A GIVEN COEFFICIENT MATRIX.
!                          = 1  IF THE COEFFICIENT MATRIX IS UNCHANGED
!                               FROM THE PREVIOUS CALL.
!
!                        WORK
!                          AN ARRAY WHICH MUST BE PROVIDED FOR
!                          WORKSPACE.  WORK MUST BE DIMENSIONED AT LEAST
!                          2*(N-1).
!
! ON OUTPUT              X
!                          AN ARRAY WHICH CONTAINS THE SOLUTION TO THE
!                          SYSTEM OF EQUATIONS.
!
!                        WORK
!                          CONTAINS INTERMEDIATE VALUES WHICH MUST NOT
!                          BE DESTROYED IF TRDI IS TO BE CALLED AGAIN
!                          WITH KS = 1.
!
! SPECIAL CONDITIONS     THIS SUBROUTINE EXECUTES SATISFACTORILY
!                        IF THE INPUT MATRIX IS DIAGONALLY DOMINANT
!                        AND NON-SINGULAR.  THE DIAGONAL ELEMENTS
!                        ARE USED TO PIVOT, AND NO TESTS ARE MADE TO
!                        DETERMINE SINGULARITY.  IF A SINGULAR, OR
!                        NEARLY SINGULAR, MATRIX IS USED AS INPUT,
!                        A DIVIDE BY ZERO OR FLOATING POINT OVERFLOW
!                        MAY RESULT.
!
! I/O                    ULIBER IS CALLED WITH AN APPROPRIATE
!                        MESSAGE IF  N  OR  KS  IS OUT OF RANGE
!
! PRECISION              SINGLE
!
! REQUIRED LIBRARY       ULIBER
! FILES
!
! LANGUAGE               FORTRAN
!
! HISTORY                ORIGINALLY WRITTEN BY NANCY WERNER AT NCAR
!                        IN OCTOBER, 1973.
!
! PORTABILITY            FORTRAN 66
!
! ALGORITHM              AN LU-DECOMPOSITION IS OBTAINED USING THE
!                        ALGORITHM DESCRIBED IN THE REFERENCE BELOW.
!
!                        TO AVOID UNNECESSARY DIVISIONS, THE ALPHA
!                        VALUES USED IN THE ROUTINE TRDSLV ARE THE
!                        RECIPROCALS OF THE ALPHA VALUES DESCRIBED
!                        IN THE REFERENCE BELOW.
!
! ACCURACY               EVERY COMPONENT OF THE RESIDUAL OF THE LINEAR
!                        SYSTEM (I.E. THE DIFFERENCE BETWEEN  Y  AND
!                        THE MATRIX APPLIED TO X) SHOULD BE LESS IN
!                        MAGNITUDE THAN TEN TIMES THE MACHINE PRECISION
!                        TIMES THE MATRIX ORDER TIMES THE MAXIMUM
!                        ABSOLUTE COMPONENT OF THE SOLUTION VECTOR
!                        TIMES THE LARGEST ABSOLUTE ROW SUM OF THE
!                        INPUT MATRIX.
!
! TIMING                 THE TIMING IS ROUGHLY PROPORTIONAL TO THE
!                        ORDER N OF THE LINEAR SYSTEM.
!
! REFERENCES             ANALYSIS OF NUMERICAL METHODS BY
!                        E. ISAACSON AND H. KELLER
!                        (JOHN WILEY AND SONS, 1966) PP. 55-58.
!***********************************************************************
!TBH      SUBROUTINE TRDI (N,A,B,C,Y,X,KS,WORK)
!TBH      DIMENSION       A(N)       ,B(N)       ,C(N)       ,Y(N)       ,  &
!TBH                      X(N)       ,WORK(1)
!TBH      SAVE
!
!     CHECK IF  N  AND  KS  ARE WITHIN THEIR SPECIFIED RANGES
!
!TBH      IF (N .GT. 2) GO TO 101
!TBH      IER = 32
!CJ   CALL ULIBER (IER,44HARRAY DIMENSION N NOT GREATER THAN 2 IN TRDI,
!CJ  1             44)
!TBH      RETURN
!TBH  101 CONTINUE
!TBH      IF (KS.EQ.0 .OR. KS.EQ.1) GO TO 102
!TBH      IER = 33
!CJ   CALL ULIBER (IER,38HKS NOT EQUAL TO 0 OR 1 IN CALL TO TRDI,38)
!TBH      RETURN
!TBH  102 CONTINUE
!
!     COMPLETE THE SOLUTION
!
!TBH      CALL TRDSLV (N,A,B,C,Y,X,KS,WORK(1),WORK(N))
!TBH      RETURN
!TBH      END


! This subprogram is used by subroutine TRDI in library ncarm.
! It is lower-level, i.e. not called by users.
! **************************************************************
      SUBROUTINE TRDSLV0(N,A,B,C,Y,X)
      IMPLICIT NONE
      INTEGER, INTENT(IN   ) :: N
      REAL,    INTENT(IN   ) :: A(N)
      REAL,    INTENT(IN   ) :: B(N)
      REAL,    INTENT(IN   ) :: C(N)
      REAL,    INTENT(IN   ) :: Y(N)
      REAL,    INTENT(  OUT) :: X(N)
      REAL :: ALPHA(N-1), GAMMA(N-1)
      INTEGER :: NM1,I,J,JB
!
      NM1 = N-1
!
!     OBTAIN THE LU-DECOMPOSITION
!
      ALPHA(1) = 1./B(1)
      GAMMA(1) = C(1)*ALPHA(1)
!TBH:  leave this on CPU until we address recurrence relations
!!!$hmppcg noParallel
      DO I=2,NM1
         ALPHA(I) = 1./(B(I)-A(I)*GAMMA(I-1))
         GAMMA(I) = C(I)*ALPHA(I)
      ENDDO
!
!     SOLVE
!
      X(1) = Y(1)*ALPHA(1)
!TBH:  leave this on CPU until we address recurrence relations
!!!$hmppcg noParallel
      DO I=2,NM1
         X(I) = (Y(I)-A(I)*X(I-1))*ALPHA(I)
      ENDDO
      X(N) = (Y(N)-A(N)*X(NM1))/(B(N)-A(N)*GAMMA(NM1))
      DO J=1,NM1
         JB = N-J
         X(JB) = X(JB)-GAMMA(JB)*X(JB+1)
      ENDDO
      RETURN
!
! REVISION HISTORY---
!
! JANUARY 1978     DELETED REFERENCES TO THE  *COSY  CARDS AND
!                  ADDED REVISION HISTORY
!-----------------------------------------------------------------------
      END SUBROUTINE TRDSLV0
