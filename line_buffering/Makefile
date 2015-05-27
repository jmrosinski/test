FC = ifort
CC = icc
OBJS = fmain.o linebuf_stdout.o

%.o: %.F90
	$(FC) -c $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) a.out
