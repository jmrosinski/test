include ../makefile.defs
COBJS   = f_wrappers.o
FOBJS   = gptlf.o
OBJS    = $(COBJS) $(FOBJS)
FFLAGS  = -g
ARFLAGS = cr

all: lib

lib: $(OBJS)
	$(AR) $(ARFLAGS) lib$(LIBNAME).a $(OBJS)

.PHONY: all clean

%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS)
