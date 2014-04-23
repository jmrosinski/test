FC = ifort
FFLAGS = -g -traceback
GPTLROOT = /home/rosinski/jet/gptl-4.0/intel
SRCS = main.F90 sub.F90 sub2.F90

a.out: $(SRCS)
	$(FC) -o $@ $(SRCS) -I$(GPTLROOT)/include $(FFLAGS) -L$(GPTLROOT)/lib -lgptl \
          -L/opt/papi/4.1.0/lib -lpapi

