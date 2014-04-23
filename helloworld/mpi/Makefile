# Set macros for default target (native)
CC = mpicc
FC = mpif90
CFLAGS = -g
FFLAGS = -g
LDFLAGS =
FLDFLAGS =

COBJS = main.o error.o sub.o
FOBJS = fmain.o
THREADED = no

ifeq ($(THREADED),yes)
  CFLAGS += -DTHREADED -mp
  LDFLAGS += -mp
endif

all: a.out a.out.fort

a.out: $(COBJS)
	$(CC) -o $@ $(COBJS) $(LDFLAGS)

a.out.fort: $(FOBJS)
	$(FC) -o $@ $(FOBJS) $(FLDFLAGS)
clean:
	$(RM) $(COBJS) $(FOBJS) a.out a.out.fort

fmain.o: fmain.f90
	$(FC) $(FFLAGS) -c -o $@ $<
