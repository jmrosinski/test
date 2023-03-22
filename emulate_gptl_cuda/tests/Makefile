include ../makefile.defs
OBJS = cputest.o
EXE  = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) -L../src -l$(LIBNAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	$(RM) $(OBJS) $(EXE)

