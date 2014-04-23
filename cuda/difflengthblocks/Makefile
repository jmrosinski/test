CC         = icc
#CFLAGS     = -g -ftz -traceback
GPU_HOME   = /whome/govett/F2C-ACC
F2C        = $(GPU_HOME)/bin/F2C-ACC
M4_INCLUDE = /whome/govett/F2C-ACC/include/logical.m4 /whome/govett/F2C-ACC/include/ARCH.m4
NVCC       = $(CUDA)/bin/nvcc
NVCC_FLAGS = -g -G -arch=sm_20
mCUDA      = /whome/govett/cuda
LIBCUDA    = -L/home/govett/cuda3/lib64 -lcudart -L/lfs0/projects/acb/govett/NVIDIA_GPU_Computing_SDK/C/lib -lcutil_x86_64

GPUOBJS1 = nprox.o
GPUOBJS2 = kprox.o

.SUFFIXES:
.SUFFIXES: .o .cu
.PRECIOUS: .cu

all: nprox kprox

nprox: $(GPUOBJS1)
	$(NVCC) -o $@ $(GPUOBJS1) $(LIBCUDA)

kprox: $(GPUOBJS2)
	$(NVCC) -o $@ $(GPUOBJS2) $(LIBCUDA)

nprox.o: nprox.cu
	$(NVCC) -c $(NVCC_FLAGS) -I$(mCUDA)/common/inc -I$(GPU_HOME)/include $<

kprox.o: kprox.cu
	$(NVCC) -c $(NVCC_FLAGS) -I$(mCUDA)/common/inc -I$(GPU_HOME)/include $<

clean:
	$(RM) $(GPUOBJS1) $(GPUOBJS2) nprox kprox
