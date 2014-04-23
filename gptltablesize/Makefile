FC = mpif90
GPTL = /lfs2/projects/fim-njet/rosinski/gptl_2bcommitted
FFLAGS = -g -I$(GPTL)
LDFLAGS = -L$(GPTL) -lgptl
OBJS = main.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $? $(LDFLAGS)
