CC = gcc
CFLAGS = -c -g
LDFLAGS = -lm
OBJS1 = predict.o
OBJS2 = change.o
EXES  = predict change

all: $(EXES)

predict: $(OBJS1)
	$(CC) -o $@ $(OBJS1) -lm

change: $(OBJS2)
	$(CC) -o $@ $(OBJS2)

clean:
	$(RM) $(EXES) $(OBJS1) $(OBJS2)
