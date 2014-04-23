FC = ifort
CC = gcc
FFLAGS = -O0 -g
CFLAGS = -O0 -g

COBJS = prbits.o
FOBJS = fmain.o
OBJS = $(COBJS) $(FOBJS)

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(FFLAGS) $*.F90

prbits: $(OBJS)
	$(FC) -g -o $@ $(OBJS)

prlaheybits: $(COBJS) prlaheybits.F90
	lf95 --chk --trap --trace --info -g -o $@ $(COBJS) prlaheybits.F90

clean:
	$(RM) $(OBJS) a.out prbits prlaheybits
