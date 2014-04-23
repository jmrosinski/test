CC = mpicc
OMPFLAG = -openmp
CFLAGS = -g -O2 $(OMPFLAG)
LDFLAGS = $(OMPFLAG)
OBJS = main.o

a.out: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
