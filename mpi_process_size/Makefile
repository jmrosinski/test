CC = mpicc
CFLAGS = -I/home/rosinski/jet/intel-mvapich/include
LDFLAGS = -L/home/rosinski/jet/intel-mvapich/lib -lgptl

a.out: main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)
