CC     = cc
CFLAGS = -g
LINKER = $(CC)
OBJS   = perf.o

all: cpr2dates

cpr2dates: $(OBJS)
	$(LINKER) -o $@ $(OBJS)

%.o: %.F90
	$(FC) -c -g $<

clean:
	$(RM) $(OBJS) cpr2dates
