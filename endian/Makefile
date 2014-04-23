CC = gcc
FC = ifort
CFLAGS = -g -O1
FFLAGS = -g -O1
COBJS = main.o
FOBJS = fmain.o

%.o: %.F90
	$(FC) -c $< $(FFLAGS)

all: endian writit

writit: $(FOBJS)
	$(FC) -o $@ $^ $(FFLAGS) $(LDFLAGS)

endian: $(COBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(COBJS) $(FOBJS) writit endian
