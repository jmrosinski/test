FC = gfortran
ifeq ($(FC),gfortran)
  FFLAGS = -fbacktrace -g -fbounds-check -Warray-bounds -finit-real=snan -ffpe-trap=invalid,zero,overflow
endif

ifeq ($(FC),ifort)
  FFLAGS = -traceback -g -check bounds,pointers,uninit -init=arrays,snan -fpe0
endif

OBJS = main.o problems.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(FFLAGS)

clean:
	$(RM) $(OBJS) *.mod a.out

%.o: %.f90
	$(FC) -c $(FFLAGS) $<

main.o: problems.o
