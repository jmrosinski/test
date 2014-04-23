FC = ifort
FFLAGS = -g
SRCS = $(shell ls *.F90)
OBJS = $(addsuffix .o, $(basename $(SRCS)))

.SUFFIXES:
.SUFFIXES: .o .F90

.F90.o:
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) a.out

external_interfaces.o: 
main.o: sizes.o external_interfaces.o
sizes.o:
sub.o: sizes.o external_interfaces.o
