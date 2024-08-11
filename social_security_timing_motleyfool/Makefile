CC = gcc
CFLAGS = -g

%.o: %.c
	$(CC) -c $(CFLAGS) $<

a.out: main.o
	$(CC) -o $@ $< $(CFLAGS)

clean: main.o a.out
	$(RM) $?

