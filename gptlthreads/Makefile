FC = mpif90
OMPFLAG = -openmp
GPTL = /home/hpc/GPTL/gptl-v5.0/mvapich
FFLAGS = -g -O3 -traceback -fpe0 $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = $(OMPFLAG) -traceback -L$(GPTL)/lib -lgptl
OBJS = main.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
