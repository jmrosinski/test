CPP = /lib/cpp -C -P
SMS = /whome/jacquesm/SMS/SMS_r108_intel-11.1-mvapich2-1.4.1
PPP = $(SMS)/bin/ppp
FC = mpif90
FFLAGS = -O0 -g -free -traceback
OBJS = decomp.o driver.o fmain.o readarr64.o writearr64.o

.SUFFIXES:
.SUFFIXES: .o .f .F90 .f90 .c

.F90.o:
	$(CPP) $< > $*_cpp.f
	$(PPP) --FreeFormat $*_cpp.f
	$(FC) -c $(FFLAGS) -I$(SMS)/include $*_cpp_sms.f
	mv $*_cpp_sms.o $*.o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) -L$(SMS)/lib -lsms

clean:
	$(RM) *.o *.f90 *.f *.tmp *.mod

decomp.o: decomp.F90
driver.o: decomp.o
fmain.o: decomp.o
readarr64.o: decomp.o
writearr64.o: decomp.o
