GPTL      = $(HOME)/install/gptl_cuda
CC        = nvc
# -acc says to recognize OpenACC directives
# -Mcuda=cc61 says to target only GPU compute capability 6.1
CACCFLAGS = -g -m64 -acc -Minfo=accel -Mcuda=cc61
CFLAGS    = -I$(GPTL)/include $(CACCFLAGS)
# Link to GPTL cpu and cuda libraries.
# -lunwind only necessary if GPTL was built with that support
LDFLAGS = $(CACCFLAGS) -L$(GPTL)/lib -lgptl -lgptl_cuda -lunwind
EXES    = ohdest warp2sm utrtest simplesum nogptl
OBJS    = $(addsuffix .o, $(EXES))

all: $(EXES)

ohdest: ohdest.o
	$(CC) -o $@ $? $(LDFLAGS)
warp2sm: warp2sm.o
	$(CC) -o $@ $? $(LDFLAGS)
utrtest: utrtest.o
	$(CC) -o $@ $? $(LDFLAGS)
simplesum: simplesum.o
	$(CC) -o $@ $? $(LDFLAGS)
nogptl: nogptl.o
	$(CC) -o $@ $? $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	$(RM) $(EXES) $(OBJS) timing.*
