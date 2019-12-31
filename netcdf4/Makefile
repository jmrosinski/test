FC = mpif90
#H5ROOT = /opt/hdf5/1.8.3-intel
FFLAGS = -g -O0
# libhdf5_hl contains "high level" hdf5 entries
# libz.a contains compression entries
#LDFLAGS = -L$(NETCDF)/lib -lnetcdff -lnetcdf -L$(H5ROOT)/lib -lhdf5_hl -lhdf5 -lz
LDFLAGS = -L$(NETCDF)/lib -lnetcdff -lnetcdf -lz
OBJS = main.o

%.o:%.F90
	$(FC) -c -I/usr/include $(FFLAGS) $<

a.out: $(OBJS)
	$(FC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)
