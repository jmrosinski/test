FC = ifort
FFLAGS = -O3 -I/home/rosinski/jet/gptl-4.0/intel/include
LDFLAGS = -L/home/rosinski/jet/gptl-4.0/intel/lib -lgptl -L/opt/papi/4.1.0/lib -lpapi

a.out: main.f90
	$(FC) $(FFLAGS) -o $@ $< $(FFLAGS) $(LDFLAGS)

clean:
	$(RM) a.out main.o
