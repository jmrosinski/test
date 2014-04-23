FC = ifort
GPTL = /home/rosinski/jet/gptl-4.1_nompi_nopapi/intel
OMPFLAG = -openmp
FFLAGS = -O3 $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = $(OMPFLAG) -L$(GPTL)/lib -lgptl
OBJS = fmain.o sub.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
