GPTL    = $(HOME)/install/gptl_cuda
NVCC    = nvcc
CC      = nvc
FC      = nvfortran
SUBDIRS = cudatests cacctests facctests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ GPTL=$(GPTL) NVCC=$(NVCC) CC=$(CC) FC=$(FC)

.PHONY: $(SUBDIRS) clean

clean:
	for dir in $(SUBDIRS); do \
          $(MAKE) -C $$dir -f Makefile $@; \
        done
