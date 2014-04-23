FC         = ifort
FFLAGS     = -g -ftz -traceback
GPU_HOME   = /whome/govett/F2C-ACC
F2C        = $(GPU_HOME)/bin/F2C-ACC
M4_INCLUDE = /whome/govett/F2C-ACC/include/logical.m4 /whome/govett/F2C-ACC/include/ARCH.m4
NVCC       = $(CUDA)/bin/nvcc
NVCC_FLAGS = -g -G -arch=sm_20
mCUDA      = /whome/govett/cuda
LIBCUDA    = -L/home/govett/cuda3/lib64 -lcudart -L/lfs0/projects/acb/govett/NVIDIA_GPU_Computing_SDK/C/lib -lcutil_x86_64

CPUOBJS = main.o
GPUOBJS = xpose_dyn2phy.o physics.o xpose_phy2dyn.o
OBJS = $(CPUOBJS) $(GPUOBJS)

.SUFFIXES:
.SUFFIXES: .F90 .o .cu .f90
.PRECIOUS: .cu

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LIBCUDA)

.F90.o:
	cpp -traditional -P -C $< > $*_cpp.f90
	$(F2C) --Generate=CUDA $*_cpp.f90
	m4 -I $(M4_INCLUDE) $*_cpp.m4 > $*.cu
	$(NVCC) -c $(NVCC_FLAGS) -I$(mCUDA)/common/inc -I$(GPU_HOME)/include $*.cu

main.o: main.F90
	$(FC) -c $(FFLAGS) main.F90

clean:
	$(RM) $(OBJS) *.cu *.f90 *.m4 a.out
