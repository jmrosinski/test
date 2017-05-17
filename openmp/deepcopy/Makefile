OMP = yes
FC = ifort
ifeq ($(OMP),yes)
  ifeq ($(FC),ifort)
    CC = icc
    OMPFLAG = -openmp
    FFLAGS = $(OMPFLAG) -g -traceback
  else
    CC = gcc
    OMPFLAG = -fopenmp
    FFLAGS = $(OMPFLAG) -g -fbacktrace
  endif
else
  OMPFLAG =
endif
LDFLAGS = $(OMPFLAG)
OBJS = fmain.o defs.o sub.o printaddr.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) *.mod a.out

fmain.o: defs.o
sub.o: defs.o
