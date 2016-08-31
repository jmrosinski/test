CC = mpicc
OMPFLAG = -fopenmp
CFLAGS = -O2 $(OMPFLAG)
LDFLAGS =
OBJS = main.o

a.out: $(OBJS)
	$(CC) -o $@ $(OMPFLAG) $? $(LDFLAGS)

clean:
	$(RM) a.out $(OBJS)
