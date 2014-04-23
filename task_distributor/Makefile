CFLAGS = -g
FFLAGS = -g
CC = mpicc
FC = mpif90

all: a.out a.out.fort

a.out: main.c
	$(CC) -o $@ $< $(CFLAGS)

a.out.fort: main.F90
	$(FC) -o $@ $< $(FFLAGS)

clean:
	$(RM) a.out a.out.fort
