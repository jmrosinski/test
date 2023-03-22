include ../../makefile.defs
OBJS = basic.o
EXE = basic

%.o: %.cu
	$(NVCC) -c $(CUFLAGS) $<

$(EXE): $(OBJS)
	$(NVCC) -o $@ $(CUFLAGS) $(OBJS) -L../../src -l$(LIBNAME) -L../src -l$(LIBNAME_CUDA)

.PHONY: all clean

clean:
	$(RM) $(EXE) $(OBJS)
