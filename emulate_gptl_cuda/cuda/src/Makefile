include ../../makefile.defs
CUOBJS  = gptl_device.o
FOBJS   = gptl_acc.o
OBJS    = $(CUOBJS) $(FOBJS)
ARFLAGS = cr

%.o: %.cu
	$(NVCC) -c $(CUFLAGS) $<
#	$(NVCC) -c -dlink $(CUFLAGS) $<

#	$(NVCC) -c $(CUFLAGS) $<
#	$(NVCC) $(CUFLAGS) -dlink -o xxx.o $@
#	mv xxx.o $@

%.o: %.F90
	$(FC) -c $(FACCFLAGS) $<

all: lib

lib: $(OBJS)
	$(AR) $(ARFLAGS) lib$(LIBNAME_CUDA).a $(OBJS)

.PHONY: all clean

clean:
	$(RM) $(OBJS) lib$(LIBNAME_CUDA).a

