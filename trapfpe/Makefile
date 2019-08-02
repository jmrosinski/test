TARGET = gnu
ifeq ($(TARGET),gnu)
  CXX = g++
  FC = gfortran
  CXXFLAGS = -g
#  CMAINFLAGS = $(CXXFLAGS) -fexceptions
  CMAINFLAGS = $(CXXFLAGS)
  FFLAGS = -g
  FMAINFLAGS = $(FFLAGS) -ffpe-trap=invalid,zero,overflow
  LDFLAGS = -lm -rdynamic
else
  ifeq ($(TARGET),intel)
    CXX = icpc
    FC = ifort
    CXXFLAGS =
    CMAINFLAGS = $(CXXFLAGS)
#    CFLAGS = -fp-trap=common
    FFLAGS = -traceback
    FMAINFLAGS = $(FFLAGS) -fpe0
    LDFLAGS = -rdynamic
  else
    ifeq ($(TARGET),pgi)
      CXX = pgc++
      FC = pgfortran
      CXXFLAGS =
      CMAINFLAGS = $(CXXFLAGS)
      FFLAGS =
      FMAINFLAGS = $(FFLAGS) -Ktrap=fp
      LDFLAGS = -rdynamic
    else
      echo "TARGET must be intel or gnu or pgi"
    endif
  endif
endif

FOBJS = fmain.o fortfunc.o cfunc.o
COBJS = main.o fortfunc.o cfunc.o handler.o

all: testc testf

testc: $(COBJS)
	$(CXX) -o $@ $(COBJS) $(LDFLAGS)

# -lstdc++ needed to successfully link cfunc.o
testf: $(FOBJS)
	$(FC) -o $@ $(FOBJS) -lstdc++

fmain.o: fmain.f90
	$(FC) -c $(FMAINFLAGS) $<

main.o: main.C
	$(CXX) -c $(CMAINFLAGS) $<

%.o:%.f90
	$(FC) -c $(FFLAGS) $<

%.o:%.C
	$(CXX) -c $(CXXFLAGS) $<

clean:
	$(RM) $(COBJS) $(FOBJS) testc testf
