CC = mpicc
CFLAGS = -g -traceback
OBJS = main.o

a.out: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
