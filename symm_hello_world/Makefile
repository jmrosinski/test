FC = mpiifort
FFLAGS = -g
OBJS = main.o

a.out: $(OBJS)
	$(FC) -o $@ $?

%.o: %.f90
	$(FC) -c $<

clean:
	$(RM) $(OBJS) a.out
