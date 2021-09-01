ifeq ($(CXX),nvc++)
  OMPFLAGS = -mp
  ACCFLAGS = -acc
  CXXFLAGS = -g -O
else
# assume g++
  OMPFLAGS = -fopenmp
  ACCFLAGS = -fopenacc -foffload=nvptx-none -fcf-protection=none -foffload=-misa=sm_35 -fno-stack-protector
  CXXFLAGS = -g -O
endif
OBJS = util.o stencil_acc.o stencil_omp.o
EXES = stencil_acc stencil_omp

all: $(EXES)

%.o: %.cxx
	$(CXX) -c $<

stencil_acc: util.o stencil_acc.o
	$(CXX) -o $@ $(CXXFLAGS) $(ACCFLAGS) $?
stencil_omp: util.o stencil_omp.o
	$(CXX) -o $@ $(CXXFLAGS) $(OMPFLAGS) $?

stencil_acc.o stencil_omp.o : dims.h util.o
stencil_acc.o: stencil_acc.cxx
	$(CXX) -c $(CXXFLAGS) $(ACCFLAGS) $<
stencil_omp.o: stencil_omp.cxx
	$(CXX) -c $(CXXFLAGS) $(OMPFLAGS) $<
clean:
	$(RM) $(OBJS) $(EXES)
