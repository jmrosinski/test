FC = mpif90
FFLAGS = -g -traceback

all: readit writit


readit: readit.o
	$(FC) -o $@ $< $(FFLAGS)

writit: writit.o
	$(FC) -o $@ $< $(FFLAGS)

test: readit writit
	env F_UFMTENDIAN="big:1" ./writit
	env F_UFMTENDIAN="big:1" mpirun -np 1 readit

readit.o: readit.f90
	$(FC) -c $< $(FFLAGS)

writit.o: writit.f90
	$(FC) -c $< $(FFLAGS)

clean:
	$(RM) *.o readit writit ninexreal8
