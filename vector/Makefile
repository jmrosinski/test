CC = icc
FC = ifort
GPTL = /home/hpc/GPTL/gptl-4.3.3/intel_nompi
CFLAGS = -DFORTRANUNDERSCORE -g -O2 -vec-report3
FFLAGS = -g -O2 -I$(GPTL)/include -vec-report3
LDFLAGS = -L$(GPTL)/lib -lgptl
FOBJS = vectest.o
COBJS = logexp.o subc.o
#FOBJS = sub.o vectest.o
#COBJS = logexp.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(FOBJS) $(COBJS)
	$(FC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) *.o a.out
