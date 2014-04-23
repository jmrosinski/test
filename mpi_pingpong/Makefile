CC = mpicc
GPTL = $(HOME)/gptl-v5.1.1/mvapich
CFLAGS = -g -O0 -traceback -I$(GPTL)/include
LDFLAGS = -L$(GPTL)/lib -lgptl
OBJS = main.o

a.out: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
