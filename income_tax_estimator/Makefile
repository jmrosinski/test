CC      = gcc
CFLAGS  = -O0 -c -g
OBJS    = estimate.o
LDFLAGS = -O0 -g -lm
EXE     = estimate

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(EXE) $(OBJS)
