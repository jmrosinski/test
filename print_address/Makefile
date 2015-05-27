FC = ifort
OMPFLAG = -openmp
# Set THREADSAFEFLAG=-recursive to fix problem
THREADSAFEFLAG = -openmp
FFLAGS = $(OMPFLAG)
CC = icc
LDFLAGS =  $(OMPFLAG)
OBJS = main.o sub.o print_address.o

%.o: %.f90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

runit: a.out
	env OMP_NUM_THREADS=2 $<

clean:
	$(RM) $(OBJS) a.out

main.o: main.f90
print_address.o: print_address.c
sub.o: sub.f90
	$(FC) -c $(THREADSAFEFLAG) $<
