CC = gcc
CFLAGS = -c -g
LDFLAGS = -lm
OBJS1 = predict.o
OBJS2 = change.o

all: predict change

predict: $(OBJS)
	$(CC) -o $@ $(OBJS1) -lm

change: $(OBJS2)
	$(CC) -o $@ $(OBJS2)

clean:
	$(RM) $(EXE) $(OBJS1) $(OBJS2)
