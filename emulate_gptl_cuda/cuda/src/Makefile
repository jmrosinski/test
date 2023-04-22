include ../../makefile.defs
CUOBJS  = gptl_device.o
FOBJS   = gptl_acc.o
OBJS    = $(CUOBJS) $(FOBJS)
ARFLAGS = cr

all: lib

%.o: %.cu
	$(NVCC) -c $< $(CUFLAGS)

%.o: %.F90
	$(FC) -c $< $(FACCFLAGS)

lib: $(OBJS)
	$(NVCC) -arch=sm_61 -dlink $(OBJS) -o gpuCode.o
# Only cacctests needs gpuCode.o. It will be linked directly w/o using the lib
	$(AR) $(ARFLAGS) lib$(LIBNAME_CUDA).a $(OBJS)
	ranlib lib$(LIBNAME_CUDA).a

.PHONY: all clean

clean:
	$(RM) $(OBJS) gpuCode.o lib$(LIBNAME_CUDA).a
