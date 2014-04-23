FC = ifort
OMPFLAG = -openmp
GPTL = 
FFLAGS = -g -O3 -mmic $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = -mmic -L$(GPTL)/lib -lgptl
OBJS = main.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)
