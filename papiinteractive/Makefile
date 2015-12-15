FC = gfortran
OMPFLAG = -fopenmp
FFLAGS = $(OMPFLAG) -O3

a.out: main.F90
	$(FC) -o $@ $(FFLAGS) $<

clean:
	$(RM) *.o

