FC = ifort
FFLAGS = -O2
OMPFLAG = -openmp

all: heap stack threadstack

heap: heap.c
	$(CC) -o $@ $<

stack: stack.f90
	$(FC) $(FFLAGS) -o $@ $<

threadstack: threadstack.f90
	$(FC) $(FFLAGS) $(OMPFLAG) -o $@ $<

clean:
	$(RM) heap stack threadstack *.o
