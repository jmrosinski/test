FC = gfortran
CC = gcc
OBJS = fmain.o printdist.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS)

%.o: %.f90
	$(FC) -c $<

clean:
	$(RM) $(OBJS) a.out
