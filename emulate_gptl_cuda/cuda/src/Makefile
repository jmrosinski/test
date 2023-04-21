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
	$(AR) $(ARFLAGS) lib$(LIBNAME_CUDA).a $(OBJS)
	ranlib lib$(LIBNAME_CUDA).a

.PHONY: all clean

clean:
	$(RM) $(OBJS) lib$(LIBNAME_CUDA).a
