CC      = mpicc
GPTL    = /home/rosinski/gptl-v5.4.5
OMPFLAG = -fopenmp
CFLAGS  = -c -g -O0 $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = $(OMPFLAG) -L$(GPTL)/lib -lgptl
SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=*.o)

a.out: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
