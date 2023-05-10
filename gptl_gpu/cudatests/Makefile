GPTL     = $(HOME)/install/gptl_cuda
NVCC     = nvcc
CPPFLAGS = -I$(GPTL)/include
CUFLAGS  = -gencode arch=compute_61,code=compute_61 -keep -src-in-ptx -lineinfo --ptxas-options=-v -ftz=true -rdc=true
# -lunwind is only required when GPTL was configured with it enabled
LDFLAGS = -L$(GPTL)/lib -lgptl -lgptl_cuda -lunwind
EXES = test1 shared driver_serial
driver_serial_SRCS = driver_serial.cu getval.cu persist.cu sleep1.cu sleep.cu subs.cu
driver_serial_OBJS = $(addsuffix .o, $(basename $(driver_serial_SRCS)))

all: $(EXES)

test1: test1.o
	$(NVCC) -o $@ $? $(CUFLAGS) $(LDFLAGS)

driver_serial: $(driver_serial_OBJS)
	$(NVCC) -o $@ $? $(CUFLAGS) $(LDFLAGS)

shared: shared.o
	$(NVCC) -o $@ $? $(CUFLAGS) $(LDFLAGS)

%.o: %.cu
	$(NVCC) -c $< $(CPPFLAGS) $(CUFLAGS)

clean:
	$(RM) $(EXES) *.o *_dlink.*  *.ptx *.ii *.module_id *.cudafe1.* *.fatbin* timing.*
