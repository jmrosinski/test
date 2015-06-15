FC = ifort
CC = gcc
ifeq ($(FC),ifort)
  OMPFLAG = -openmp
endif

ifeq ($(FC),gfortran)
  OMPFLAG = -fopenmp
endif

OBJS = driver.o set_affinity.o print_affinity.o runnable.o

a.out: $(OBJS)
	$(FC) -o $@ $(OMPFLAG) $(OBJS)

clean:
	$(RM) $(OBJS) *.mod a.out

%.o: %.c
	$(CC) -g -c $(OMPFLAG) $<
%.o: %.F90
	$(FC) -g -c $(OMPFLAG) $<

