CC = cc
CFLAGS = -g
OBJS1 = main.o
OBJS2 = cpr2dates.o

all: bondincome cpr2dates

bondincome: $(OBJS1)
	$(CC) -o $@ $(OBJS1)
cpr2dates: $(OBJS2)
	$(CC) -o $@ $(OBJS2)
clean:
	$(RM) $(OBJS) bondincome cpr2dates
