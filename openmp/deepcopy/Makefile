OMP = yes
FC = ifort
ifeq ($(OMP),yes)
  ifeq ($(FC),ifort)
    OMPFLAG = -openmp
  else
    OMPFLAG = -fopenmp
  endif
else
  OMPFLAG =
endif
FFLAGS = $(OMPFLAG) -g -traceback
LDFLAGS = $(OMPFLAG)
OBJS = fmain.o defs.o sub.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) *.mod a.out

fmain.o: defs.o
sub.o: defs.o
