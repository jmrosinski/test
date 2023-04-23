include ../makefile.defs
CFLAGS += -I../src
OBJS = cputest.o
EXE  = $(basename $(OBJS))

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) -L../src -l$(LIBNAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: check clean

check: $(EXE)
	./$(EXE)

clean:
	$(RM) $(OBJS) $(EXE)

