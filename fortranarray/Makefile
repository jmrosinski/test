FC = gfortran
OBJS = subimp.o subs.o main.o

a.out: $(OBJS)
	$(FC) -o $@ $?

%.o: %.f90
	$(FC) -c -g $<

main.o: main.f90 subs.o
subimp.o: subimp.f90
subs.o: subs.f90

clean:
	$(RM) *.o a.out
