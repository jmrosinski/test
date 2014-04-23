CC       = mpicc
CCSERIAL = icc
CFLAGS   = -g -O3 -fexceptions -traceback
GPTL     = /whome/rosinski/jet/gptl-4.2/intel

all: treesum synthetic synthetic_printwho mean_sigma jr_reduce

treesum: treesum.c
	$(CC) $(CFLAGS) -o $@ $< -lm

synthetic: synthetic.c
	$(CCSERIAL) $(CFLAGS) -o $@ $< -lm

synthetic_printwho: synthetic_printwho.c
	$(CCSERIAL) $(CFLAGS) -o $@ $< 

mean_sigma: mean_sigma.c
	$(CCSERIAL) $(CFLAGS) -o $@ $< -lm

jr_reduce: jr_reduce.c
	$(CC) -I$(GPTL)/include $(CFLAGS) -o $@ $< -L$(GPTL)/lib -lgptl

clean:
	$(RM) *.o treesum synthetic synthetic_printwho mean_sigma jr_reduce
