#FC = ifort
#FC = pgf90
#FC = gfortran
FC = lf95
FFLAGS = -g -O0
OBJS = argv.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $?

clean:
	$(RM) $(OBJS) a.out
