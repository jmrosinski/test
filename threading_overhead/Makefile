FC = ifort
GPTL = /home/hpc/GPTL/gptl-4.3.3/intel_nompi
FFLAGS = -O3 -g -openmp -I$(GPTL)/include
SRCS = main.F90

a.out: $(SRCS)
	$(FC) $(FFLAGS) -o $@ $(SRCS) -L$(GPTL)/lib -lgptl

clean:
	$(RM) a.out

