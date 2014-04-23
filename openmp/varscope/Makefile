FC = ifort
CC = icc
FFLAGS = -g -O0 -openmp -traceback -DDYNAMIC -heap-arrays
LDFLAGS = -openmp
OBJS = main.o sub.o printmyaddress.o zzz.o

a.out: $(OBJS)
	$(FC) $(LDFLAGS) -o $@ $(OBJS)

%.o : %.F90
	$(FC) -c $(FFLAGS) $<

#sub.o: sub.F90
#	$(FC) -c -g -traceback $<

clean:
	$(RM) $(OBJS) a.out

sub.o: zzz.o
