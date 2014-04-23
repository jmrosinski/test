CC = mpicc
OMPFLAG = -openmp
CFLAGS = -g -O2 $(OMPFLAG)
LDFLAGS = $(OMPFLAG)
OBJS = xthi.o

xthi: $(OBJS)
	$(CC) -o $@ $? $(LDFLAGS)

clean:
	$(RM) xthi $(OBJS)
