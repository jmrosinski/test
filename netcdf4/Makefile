FC = mpif90
NCROOT = /opt/netcdf/4.1.1-shared
#H5ROOT = /opt/hdf5/1.8.3-intel
INCFLAGS = -I$(NCROOT)/include
FFLAGS = -g -O0
# libhdf5_hl contains "high level" hdf5 entries
# libz.a contains compression entries
#LDFLAGS = -L$(NCROOT)/lib -lnetcdff -lnetcdf -L$(H5ROOT)/lib -lhdf5_hl -lhdf5 -lz
LDFLAGS = -L$(NCROOT)/lib -lnetcdff -lnetcdf -lz
OBJS = main.o

.SUFFIXES:
.SUFFIXES: .F90 .o

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

.F90.o:
	$(FC) -c $(INCFLAGS) $(FFLAGS) $<

clean:
	$(RM) $(OBJS)
