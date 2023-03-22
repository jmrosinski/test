include ./makefile.defs
# Order of subdirs matters
SUBDIRS = ./src ./tests ./cuda ./cacctests ./facctests
CLEANDIRS = ./src ./tests ./cuda/src ./cuda/tests ./cacctests ./facctests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: $(SUBDIRS) clean

clean:
	for dir in $(CLEANDIRS); do \
          $(MAKE) -C $$dir -f Makefile $@; \
        done
