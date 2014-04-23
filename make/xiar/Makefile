FC = ifort
FFLAGS = -g -mmic
LDFLAGS = -mmic
SRCS = main.F90
OBJS = main.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

all: tst a.out

tst:
	$(MAKE) -C libdir FC=$(FC) FFLAGS="$(FFLAGS)"

a.out: $(OBJS) tst
	$(FC) -o $@ $(OBJS) $(LDFLAGS) -L./libdir -lzzz

clean:
	$(RM) $(OBJS) a.out
	$(MAKE) -C libdir clean
