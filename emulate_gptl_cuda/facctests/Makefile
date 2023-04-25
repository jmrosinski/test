include ../makefile.defs
LDFLAGS  = $(FACCFLAGS) -L../src -l$(LIBNAME) -L../cuda/src -l$(LIBNAME_CUDA)
OBJS     = basic.o
EXE      = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.F90
	$(FC) -c $< -I../src -I../cuda/src $(FACCFLAGS)

.PHONY: check clean

check: $(EXE)
	./$(EXE)

clean:
	$(RM) $(OBJS) $(EXE)
