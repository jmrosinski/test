FC = gfortran
OMPFLAG = -fopenmp
FFLAGS = -g -O2 $(OMPFLAG)
LDFLAGS = $(OMPFLAG)
OBJS = junk.o

a.out: $(OBJS)
	$(FC) $(LDFLAGS) -o $@ $(OBJS)

%.o : %.F90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS) a.out
