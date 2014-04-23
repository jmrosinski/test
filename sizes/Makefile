FC = ifort
CC = icc
OBJS = fmain.o csizes.o fsizes.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS)

fmain.o: csizes.o fsizes.o
	$(FC) -c fmain.f90
csizes.o: 
fsizes.o:
