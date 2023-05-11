GPTL = $(HOME)/install/gptl_cuda
NVCC = nvcc
CPPFLAGS = -I$(GPTL)/include
CUFLAGS  = -gencode arch=compute_61,code=compute_61 -keep -src-in-ptx -lineinfo --ptxas-options=-v -ftz=true -rdc=true
LDFLAGS = -L$(GPTL)/lib -lgptl -lgptl_cuda -lunwind

a.out: main.o
	$(NVCC) -o $@ $? $(CUFLAGS) $(LDFLAGS)

%.o: %.cu
	$(NVCC) -c $< $(CPPFLAGS) $(CUFLAGS)

clean:
	$(RM) a.out *.o *_dlink* *.fatbin* *cudafe1* *.ii
