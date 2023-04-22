include ../makefile.defs
# gpuCode.o should be in $(LIBNAME_CUDA), but that causes problems
LDFLAGS  = -L../src -l$(LIBNAME) -L../cuda/src -l$(LIBNAME_CUDA) -cudalib=nvshmem -ta=tesla,cc61
OBJS     = basic.o ../cuda/src/gpuCode.o
EXE      = basic

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -I../src -I../cuda/src $(CACCFLAGS)

clean:
	$(RM) $(OBJS) $(EXE)
