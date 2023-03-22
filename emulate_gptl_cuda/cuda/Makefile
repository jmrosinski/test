# Order of subdirs matters
SUBDIRS = ./src ./tests

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS) clean
