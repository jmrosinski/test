include ../makefile.defs
LDFLAGS  = $(CACCFLAGS) -L../src -l$(LIBNAME) -L../cuda/src -l$(LIBNAME_CUDA)
OBJS     = basic.o
EXE      = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -I../src -I../cuda/src $(CACCFLAGS)

.PHONY: check clean

check: $(EXE)
	./$(EXE)

clean:
	$(RM) $(OBJS) $(EXE)
