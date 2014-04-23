FC = ifort
CC = icc
FFLAGS = -g -O2 -openmp -auto
LDFLAGS = -openmp
OBJS = main.o sub.o sub2inmod.o getaddress.o

a.out: $(OBJS)
	$(FC) $(LDFLAGS) -o $@ $(OBJS)

%.o : %.F90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS) a.out

main.o: sub2inmod.o
