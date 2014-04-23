FC = mpif90
OMPFLAG = -openmp
GPTLDIR = /misc/whome/rosinski/jet/gptl-4.0/intel
FFLAGS = -g -O3 $(OMPFLAG) -I$(GPTLDIR)/include -fpe0
LDFLAGS = -L$(GPTLDIR)/lib -lgptl -L/opt/papi/4.1.0/lib -lpapi

a.out: main.f90
	$(FC) $(FFLAGS) -o $@ main.f90 $(LDFLAGS)

clean:
	$(RM) a.out

test:
	@tweak_hostfile -n 1
	@echo "Running 2 tasks, 8 threads per task:"
	mpirun -np 2 MV2_ENABLE_AFFINITY=0 OMP_NUM_THREADS=8 ./a.out

	@tweak_hostfile -n 1
	@echo "Running 4 tasks, 8 threads per task:"
	mpirun -np 4 MV2_ENABLE_AFFINITY=0 OMP_NUM_THREADS=8 ./a.out

	@tweak_hostfile -n 1
	@echo "Running 8 tasks, 8 threads per task:"
	mpirun -np 8 MV2_ENABLE_AFFINITY=0 OMP_NUM_THREADS=8 ./a.out

