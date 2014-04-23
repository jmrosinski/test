FC = mpiifort
FFLAGS = -g -mmic
OBJS = main.o

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $?

%.o: %.f90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS) a.out
