FC         = ifort
#FFLAGS     = -g -O0 -traceback -check bounds -I/home/rosinski/jet/gptl-4.0/intel/include
FFLAGS     = -O3 -I/home/rosinski/jet/gptl-4.0/intel/include
LDFLAGS    = -O3 -L/home/rosinski/jet/gptl-4.0/intel/lib -lgptl -L/opt/papi/4.1.0/lib -lpapi
ifeq ($(TARGET),gpu)
  CPP_FLAGS  = -DGPU
else
  FFLAGS += -openmp
  LDFLAGS += -openmp
endif
GPU_HOME   = /whome/govett/F2C-ACC_V3
F2C        = $(GPU_HOME)/bin/F2C-ACC
M4_INCLUDE = /whome/govett/F2C-ACC_V3/include/logical.m4 /whome/govett/F2C-ACC_V3/include/ARCH.m4
NVCC       = $(CUDA)/bin/nvcc
NVCC_FLAGS = -arch=sm_20
mCUDA      = /whome/govett/cuda
LIBCUDA    = -L/home/govett/cuda3/lib64 -lcudart -L/lfs0/projects/acb/govett/NVIDIA_GPU_Computing_SDK/C/lib -lcutil_x86_64

CPUOBJS = main.o
GPUOBJS = xpose_direct_stridedwrite.o \
          copyin.o copyin2.o \
          copyout.o copyout2.o
OBJS = $(CPUOBJS) $(GPUOBJS)

.SUFFIXES:
.SUFFIXES: .F90 .o .cu .f90
.PRECIOUS: .cu

gpu: a.out

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS) $(LIBCUDA)

ifeq ($(TARGET),gpu)
.F90.o:
	cpp $(CPP_FLAGS) -traditional -P -C $< > $*_cpp.f90
	$(F2C) --Generate=CUDA --Constant M=10242,N=192,MM=64,NN=96,NITER=10,CHUNKSIZE=1,NCHUNKS=1 $*_cpp.f90
	m4 -I $(M4_INCLUDE) $*_cpp.m4 > $*.cu
	$(NVCC) -c $(NVCC_FLAGS) -I$(mCUDA)/common/inc -I$(GPU_HOME)/include $*.cu
else
.F90.o:
	$(FC) -c $(CPP_FLAGS) $(FFLAGS) $<
endif

main.o: main.F90
	$(FC) -c $(CPP_FLAGS) $(FFLAGS) main.F90

clean:
	$(RM) $(OBJS) *.cu *.f90 *.m4 a.out
