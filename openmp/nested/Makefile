FC = ifort
#CPPFLAG = -DOLDWAY
CPPFLAG =
ifeq ($(CPPFLAG),-DOLDWAY)
  OMPFLAG =
else
  OMPFLAG = -openmp
endif
FFLAGS = $(CPPFLAG) $(OMPFLAG) -g -O0
LDFLAGS = $(OMPFLAG)
OBJS = main.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

clean:
	$(RM) $(OBJS) a.out
