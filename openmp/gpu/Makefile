# Installed gcc-14 with nvidia offload capability by doing this:
# sudo apt install nvptx-tools libgomp-plugin-nvptx1 gcc-14-offload-nvptx

CC = gcc-12
OBJS = main.o saxpy.o saxpy2.o saxpy3.o saxpy_1copy.o
EXE = testit
GPTL = /home/rosinski/install

# the -fcf-protection is needed or gcc fails. I don't really know what it does
# -fstack-protector is needed to overcome "Unknown symbol" error
ifeq ($(GPU),yes)
  CFLAGS = -I$(GPTL)/include -g -O2 -fopenmp -foffload=nvptx-none -fcf-protection=none -fstack-protector
else
  CFLAGS = -I$(GPTL)/include -g -O2
endif
# Added -static due to websearch indicating that having multiple compilers installed confuses
# the runtime linking (e.g. gcc-14 using gcc-12 entries)

LDFLAGS = $(CFLAGS)
LDFLAGS += -fopenmp -L$(GPTL)/lib -lgptl -lunwind
LINKCMD = $(CC) -o $@ $(OBJS) $(LDFLAGS)

$(EXE): $(OBJS)
	$(LINKCMD)

clean:
	$(RM) $(OBJS) $(EXE)
