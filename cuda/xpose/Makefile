FC         = ifort
GPU_HOME   = /whome/govett/F2C-ACC
F2C        = $(GPU_HOME)/bin/F2C-ACC
M4_INCLUDE = /whome/govett/F2C-ACC/include/logical.m4 /whome/govett/F2C-ACC/include/ARCH.m4
NVCC       = $(CUDA)/bin/nvcc
NVCC_FLAGS = -pg -arch=sm_20
mCUDA      = /whome/govett/cuda
LIBCUDA    = -L/home/govett/cuda3/lib64 -lcudart -L/lfs0/projects/acb/govett/NVIDIA_GPU_Computing_SDK/C/lib -lcutil_x86_64

OBJS = xpose.o sub.o

.SUFFIXES:
.SUFFIXES: .F90 .o .cu .f90

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LIBCUDA)

xpose.o: xpose.F90
	$(FC) -c xpose.F90

sub.o: sub.F90
	cpp -traditional -P -C sub.F90 > sub_cpp.f90
	$(F2C) --Generate=CUDA sub_cpp.f90
	m4 -I $(M4_INCLUDE) sub_cpp.m4 > sub.cu
	$(NVCC) -c $(NVCC_FLAGS) -I$(mCUDA)/common/inc -I$(GPU_HOME)/include $*.cu

clean:
	$(RM) $(OBJS) *.cu *.f90 *.m4
