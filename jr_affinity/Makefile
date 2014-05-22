CC = mpicc
OMPFLAG = -fopenmp
CFLAGS = -g -O2 $(OMPFLAG)
LDFLAGS = $(OMPFLAG)
OBJS1 = main.o runnable.o
OBJS2 = setaff.o runnable.o

all: a.out setaff

a.out: $(OBJS1)
	$(CC) -o $@ $+ $(LDFLAGS)

setaff: $(OBJS2)
	$(CC) -o $@ $+ $(LDFLAGS)

clean:
	$(RM) a.out setaff $(OBJS1) $(OBJS2)
