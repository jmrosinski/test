FC = ifort
FFLAGS = -g
SRCS = main.F90 dffusn.F90
OBJS = $(addsuffix .o, $(basename $(SRCS)))

.SUFFIXES:
.SUFFIXES: .o .F90

.F90.o:
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $(OBJS)

dffusn.o: dffusn.F90
main.o: main.F90 dffusn.o
