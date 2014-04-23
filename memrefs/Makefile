FC = mpif90
FFLAGS = -O2 -I/home/rosinski/jet/intel/single_threaded/include
LDFLAGS = -L/home/rosinski/jet/intel/single_threaded/lib -lgptl -L/opt/papi/4.1.0/lib -lpapi

a.out: memrefs.F90
	$(FC) -o $@ $< $(FFLAGS) $(LDFLAGS)

