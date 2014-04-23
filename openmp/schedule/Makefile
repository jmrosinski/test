FC           = mpif90
CC           = icc
GPTL         = /home/rosinski/jet/gptl-4.1/intel
PAPI_LDFLAGS = -L/opt/papi/4.1.0/lib -lpapi
GPTL_LDFLAGS = -L$(GPTL)/lib -lgptl $(PAPI_LDFLAGS)
OMPFLAG      = -openmp
CFLAGS       = -g -traceback $(OMPFLAG)
FFLAGS       = -g -traceback $(OMPFLAG) -I$(GPTL)/include
OBJS         = main.o subs.o
LDFLAGS      = $(OMPFLAG) $(GPTL_LDFLAGS) $(PAPI_LDFLAGS)

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.F90
	$(FC) -c $(FFLAGS) -o $@ $*.F90

clean:
	$(RM) $(OBJS) a.out
