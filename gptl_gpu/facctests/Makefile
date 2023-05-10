GPTL      = $(HOME)/install/gptl_cuda
FC        = nvfortran
# -acc says to recognize OpenACC directives
# -Mcuda=cc61 says to target only GPU compute capability 6.1
FACCFLAGS = -g -m64 -acc -Minfo=accel -Mcuda=cc61
FCFLAGS   = -I$(GPTL)/include $(FACCFLAGS)
# Link to GPTL cpu and cuda libraries.
# -lunwind only necessary if GPTL was built with that support
LDFLAGS     = $(FACCFLAGS) -L$(GPTL)/lib -lgptlf -lgptl -lgptl_cuda -lunwind
EXES        = driver_vdmints3 warp2sm utrtest
DRIVER_OBJS = vdmints3_sim.o getval.o
OBJS        = $(addsuffix .o, $(EXES)) $(DRIVER_OBJS)

all: $(EXES)

driver_vdmints3: driver_vdmints3.o $(DRIVER_OBJS)
	$(FC) -o $@ $? $(LDFLAGS)
warp2sm: warp2sm.o
	$(FC) -o $@ $? $(LDFLAGS)
utrtest: utrtest.o
	$(FC) -o $@ $? $(LDFLAGS)

%.o: %.F90
	$(FC) -c $< $(FCFLAGS)

clean:
	$(RM) $(EXES) $(OBJS) *.mod timing.*

driver_mpi.o: getval.o
driver_serial.o: getval.o
driver_vdmints3.o: getval.o
