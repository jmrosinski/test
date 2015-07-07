FC = ifort
CC = icc
CFLAGS = 
FFLAGS = -O2
OMPFLAG = -openmp

#all: heap stack threadstack
all: heap

heap: heap.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	$(RM) heap *.o
