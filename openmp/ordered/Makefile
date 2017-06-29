FC = gfortran
OMPFLAG = -fopenmp
FFLAGS = $(OMPFLAG) -g -O2
LDFLAGS = $(OMPFLAG)
OBJS = main.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS) a.out
