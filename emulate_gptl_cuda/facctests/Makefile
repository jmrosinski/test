include ../makefile.defs
FLDFLAGS = -Mcuda=cc61 -lstdc++
OBJS     = basic.o
EXE      = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(FC) -o $@ $(OBJS) $(FACCFLAGS) $(FLDFLAGS) -L../src -l$(LIBNAME) -L../cuda/src -l$(LIBNAME_CUDA)

%.o: %.F90
	$(FC) -c $< -I../src -I../cuda/src $(FACCFLAGS)

.PHONY: check clean

check: $(EXE)
	./$(EXE)

clean:
	$(RM) $(OBJS) $(EXE)
