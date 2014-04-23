FC = ifort
FFLAGS =
OBJS = main.o sub.o dims.o

.SUFFIXES:
.SUFFIXES: .f90 .o

.f90.o:
	$(FC) -c -o $@ $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) a.out *.mod

main.o: dims.o
dims.o:
sub.o: dims.o
