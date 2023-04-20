include ../makefile.defs
LDFLAGS  = -L../src -l$(LIBNAME) -L../cuda/src -l$(LIBNAME_CUDA) -cudalib=nvshmem -ta=tesla,cc61
OBJS     = basic.o
EXE      = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -I../src -I../cuda/src $(CACCFLAGS)

clean:
	$(RM) $(OBJS) $(EXE)
