FC = ifort
FFLAGS = -g
OBJS = main.o junk.o

%.o: %.F90
	$(FC) -c $<

a.out: $(OBJS)
	$(FC) -o $@ $?

main.o: junk.o
