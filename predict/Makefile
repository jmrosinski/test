CC = gcc
CFLAGS = -c -g
LDFLAGS = -lm
OBJS = predict.o
EXE = predict

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

clean:
	$(RM) $(EXE) $(OBJS)
