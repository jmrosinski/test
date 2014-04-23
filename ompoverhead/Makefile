GPTL = /home/hpc/GPTL/gptl-4.3.3/intel_nompi
FC = ifort
OMPFLAG = -openmp
FFLAGS = -g -O2 -traceback -fpe0 $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = $(OMPFLAG) -L$(GPTL)/lib -lgptl
SRCS = main.F90 subs.F90
OBJS = $(addsuffix .o, $(basename $(SRCS)))

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

all: $(OBJS)
	$(FC) -o a.out $(OBJS) $(OMPFLAG) $(LDFLAGS)

main.o: subs.o

clean:
	$(RM) $(OBJS) a.out
