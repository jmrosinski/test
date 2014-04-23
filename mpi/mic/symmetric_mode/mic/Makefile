FC = mpiifort
FFLAGS = -g -O2 -traceback -mmic -openmp
OBJS = main.o

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $?

clean:
	$(RM) $(OBJS) a.out

%.o: %.F90
	$(FC) -c $(FFLAGS) $<
