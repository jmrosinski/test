CC = icc
FC = ifort
CFLAGS = -O3 -I/home/rosinski/jet/intel/single_threaded/include
FFLAGS = -msse3 -O5 -I/home/rosinski/jet/intel/single_threaded/include

LDFLAGS = -L/home/rosinski/jet/intel/single_threaded/lib -lgptl -L/opt/papi/4.1.0/lib -lpapi

all: a.out a.out.fort

a.out: main.o
	$(CC) -o $@ main.o $(LDFLAGS)

a.out.fort: fmain.F90
	$(FC) -o $@ fmain.F90 $(FFLAGS) $(LDFLAGS)

clean:
	$(RM) main.o a.out a.out.fort

