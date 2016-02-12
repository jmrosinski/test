FC = ifort
FFLAGS = -qopt-report-phase=vec
OBJS = main.o funcs.o

%.o: %.F90
	$(FC) -c $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $^

clean:
	$(RM) $(OBJS) a.out

main.o: funcs.o
