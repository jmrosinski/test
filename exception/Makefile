FC = ifort

FFLAGS = -g

ifeq ($(FC),gfortran)
  FFLAGS += -ffpe-trap=invalid,overflow,zero
endif

ifeq ($(FC),xlf90)
  FFLAGS += -qflttrap=zerodivide:invalid:enable
endif

ifeq ($(FC),ifort)
  FFLAGS += -fpe0 -ftz -traceback
endif

a.out: main.f90
	$(FC) $(FFLAGS) $<

clean:
	$(RM) *.o a.out

