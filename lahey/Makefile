FC = lf95
FFLAGS = --chk -g --trap --trace --info
OBJS = infnan.o main.o sub.o

.SUFFIXES:
.SUFFIXES: .F90 .o

a.out: $(OBJS)
	$(FC) $(FFLAGS) -o $@ $(OBJS)

.F90.o:
	$(FC) -c $(FFLAGS) $<

main.o: infnan.o

clean:
	$(RM) *.o
