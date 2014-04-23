FC = ifort
OBJS = mymod.o main.o

%.o: %.f90
	$(FC) -c $<

a.out: $(OBJS)
	$(FC) -o $@ $?

clean:
	$(RM) $(OBJS) a.out

mymod.o:
main.o: mymod.o
