CFLAGS = -O

EXEDIR = $(HOME)/bin
EXENAME = sysconfjr

UNAMES := $(shell uname -s)
UNAMEM := $(findstring CRAY,$(shell uname -m))

OBJS = main.o

ifeq ($(UNAMEM),CRAY)
  CFLAGS += -DCRAY
endif

ifeq ($(UNAMES),IRIX64)
  CFLAGS += -DSGI -64
endif

ifeq ($(UNAMES),Linux)
  CFLAGS += -DLINUX
endif

ifeq ($(UNAMES),AIX)
  CFLAGS += -DAIX
endif

$(EXEDIR)/$(EXENAME) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o

main.o: main.c

