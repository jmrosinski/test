FC = ifort
#GPTL = /home1/02455/tg817513/gptl-v5.3.2/xeonphi_nompi
GPTL = /misc/whome/hpc/GPTL/gptl-v5.3.1/intel_nompi
MICFLAG = 
ifeq ($(FC),ifort)
  OMPFLAG = -openmp
endif
ifeq ($(FC),gfortran)
  OMPFLAG = -fopenmp
endif
FFLAGS = $(MICFLAG) $(OMPFLAG) -g -O2 -I$(GPTL)/include
LDFLAGS = $(MICFLAG) $(OMPFLAG) -L$(GPTL)/lib -lgptl
OBJS = main.o parms.o subs.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) *.mod a.out

main.o: parms.o subs.o
subs.o: parms.o
