CC = gcc
CFLAGS = -c -g
LDFLAGS = -lm
OBJS1 = predict.o
OBJS2 = change.o
OBJS3 = invest_growth.o
EXES  = predict change invest_growth

all: $(EXES)

predict: $(OBJS1)
	$(CC) -o $@ $(OBJS1) -lm

change: $(OBJS2)
	$(CC) -o $@ $(OBJS2)

invest_growth: $(OBJS3)
	$(CC) -o $@ $(OBJS3)

clean:
	$(RM) $(EXES) $(OBJS1) $(OBJS2) $(OBJS3)
