FC = mpif90
ifeq ($(TARGET),pgi)
FFLAGS = -O3 -I/home/rosinski/jet/pgi/include
LDFLAGS = -llapack
LDFLAGS += -L/home/rosinski/jet/pgi/lib -lgptl -lpthread -L/opt/papi/4.1.0/lib -lpapi
else
FFLAGS = -O3 -I/home/rosinski/jet/intel/include
LDFLAGS = -L$(MKL) -lmkl_intel_lp64 -lmkl_sequential -lmkl_core
LDFLAGS += -L/home/rosinski/jet/intel/lib -lgptl -lpthread -L/opt/papi/4.1.0/lib -lpapi
endif
OBJS = main.o solve.o trdi.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out

.F90.o:
	$(FC) -c $(FFLAGS) $<

main.o: solve.o main.F90
	$(FC) -c -o $@ $*.F90 $(FFLAGS)
solve.o: solve.F90
	$(FC) -c -o $@ $*.F90 $(FFLAGS)
trdi.o: trdi.F90
	$(FC) -c -o $@ $*.F90 $(FFLAGS)
