SRCS = sub1.F90 sub2.F90
OBJS = sub1.o sub2.o
LIBNAME = zzz
ARFLAGS = ruv

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

all: $(OBJS)
	$(AR) $(ARFLAGS) lib$(LIBNAME).a $(OBJS)

clean:
	$(RM) $(OBJS) lib$(LIBNAME).a


