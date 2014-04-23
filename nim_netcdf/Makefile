FC = mpif90
GPTL = /home/hpc/GPTL/gptl-v5.0/mvapich
FFLAGS = -g -O3 -traceback -fpe0 -I$(NETCDF)/include -I$(GPTL)/include
LDFLAGS = -L$(NETCDF)/lib -lnetcdf -L$(GPTL)/lib -lgptl
OBJS = handle_err.o writenc.o readnc.o

%.o: %.F90
	$(FC) -c $< $(FFLAGS)

all: writenc readnc

writenc: handle_err.o writenc.o
	$(FC) -o $@ $^ $(LDFLAGS)

readnc: handle_err.o readnc.o
	$(FC) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out

writenc.o: handle_err.o
readnc.o: handle_err.o
