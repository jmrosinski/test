include ../../makefile.defs
OBJS = basic.o
EXE = basic

$(EXE): $(OBJS)
	$(NVCC) -o $@ $(CUFLAGS) $(OBJS) -L../../src -l$(LIBNAME) -L../src -l$(LIBNAME_CUDA)

%.o: %.cu
	$(NVCC) -c $(CUFLAGS) $<

.PHONY: check clean

check: $(EXE)
	./$(EXE)

clean:
	$(RM) $(EXE) $(OBJS)
