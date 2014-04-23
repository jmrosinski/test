FC = ifort
FFLAGS = -g -traceback
OBJS1 = module_control.o module_header.o writit.o
OBJS2 = module_control.o readit.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

all: writit readit

writit: $(OBJS1)
	$(FC) -o $@ $? $(FFLAGS)

readit: $(OBJS2)
	$(FC) -o $@ $? $(FFLAGS)

clean:
	$(RM) *.o *.mod readit writit

module_header.o: module_control.o
writit.o: module_header.o module_control.o
readit.o: module_control.o
