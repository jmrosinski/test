include ./makefile.defs
# Order of subdirs matters
SUBDIRS = ./src ./tests ./cuda ./cacctests ./facctests
CLEANDIRS = ./src ./tests ./cuda/src ./cuda/tests ./cacctests ./facctests
CHECKDIRS = ./tests ./cuda/tests ./cacctests ./facctests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

check:
	for dir in $(CHECKDIRS); do \
          $(MAKE) check -C $$dir -f Makefile $@; \
        done

.PHONY: $(SUBDIRS) clean

clean:
	for dir in $(CLEANDIRS); do \
          $(MAKE) -C $$dir -f Makefile $@; \
        done
