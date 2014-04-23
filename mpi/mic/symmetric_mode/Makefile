FC = mpiifort
FFLAGS = -g -O2 -traceback -openmp
OBJS = main.o

all: a.out mic/a.out

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $?

mic/a.out:
	$(MAKE) -C mic

clean:
	$(RM) $(OBJS) a.out
	$(MAKE) clean -C mic

%.o: %.F90
	$(FC) -c $(FFLAGS) $<
