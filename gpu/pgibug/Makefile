FC      = pgfortran
CC      = pgcc
FFLAGS  = -acc -Minfo=accel -Minfo -ta=tesla:cc60 -Mcuda=cuda8
CFLAGS  = $(FFLAGS)
#JR -DCINTFC makes driver call CPU-based C code which invokes CUDA code (fails)
#JR Without this flag drive will call CUDA directly (works)
ifeq ($(FLAGS),fail)
  FFLAGS += -DCINTFC
endif
CU      = nvcc
CUFLAGS = -G -O0 -gencode arch=compute_60,code=compute_60 -rdc=true
LDFLAGS = -ta=tesla:cc60 -Mcuda=cuda8,rdc
OBJS    = driver.o subs.o ccode_.o

%.o: %.c
	$(CC) -c $< $(CFLAGS)

%.o: %.cu
	$(CU) -c $< $(CUFLAGS)

%.o: %.F90
	$(FC) -c $< $(FFLAGS)

all: $(OBJS)
	$(FC) -o a.out $? $(LDFLAGS)

clean:
	$(RM) $(OBJS) a.out
