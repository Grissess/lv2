# TODO: find a better way to detect the suffix
LIB_EXT ?= .so
NXS_URI ?= http://nexusg.org/lv2/plugins/
export LIB_EXT NXS_URI

DIRS = $(filter-out Makefile rules.mk,$(wildcard *))

all: $(DIRS)

$(DIRS):
	$(MAKE) -C "$@"

.PHONY: all $(DIRS)
