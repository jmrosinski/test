GPTL = /usr/local/gptl-v5.4.4
CC = mpicc
OMPFLAG = -fopenmp
CFLAGS = -g -O0 $(OMPFLAG) -I$(GPTL)/include
LDFLAGS = $(OMPFLAG) -L$(GPTL)/lib -lgptl -lm
OBJS1 = main.o dowork.o runnable.o
OBJS2 = setaff.o runnable.o

all: a.out setaff

a.out: $(OBJS1)
	$(CC) -o $@ $^ $(LDFLAGS)

setaff: $(OBJS2)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) a.out setaff $(OBJS1) $(OBJS2)
