FC = ifort
ifeq ($(FC),gfortran)
  FFLAGS = -g -O0 -fopenmp -fno-automatic
  LDFLAGS = -fopenmp -L/home/hpc/EFENCE/electric-fence-2.1.13/lib -lefence
else
  FFLAGS = -g -O0 -heap-arrays
  LDFLAGS = -L/home/hpc/DUMA/lib -lduma
endif
OBJS = main.o

%.o: %.F90
	$(FC) -c $< $(FFLAGS)

a.out: $(OBJS)
	$(FC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
