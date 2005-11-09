#
# Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
#

TOPDIR = ..
include $(TOPDIR)/include/builddefs

README = README.xfsdump
LSRCFILES = INSTALL PORTING CHANGES COPYING $(README) \
	directories.gif directories.obj               \
	files.gif files.obj                           \
	global_hdr.gif global_hdr.obj                 \
	inode_map.gif inode_map.obj                   \
	media_files.gif media_files.obj               \
	split_algorithm.gif split_algorithm.obj       \
	xfsdump.html xfsdump_ts.txt

LDIRT = *.gz

default: CHANGES.gz

include $(BUILDRULES)

CHANGES.gz:
	$(ZIP) --best -c < CHANGES > $@

install: default
	$(INSTALL) -m 755 -d $(PKG_DOC_DIR)
	$(INSTALL) -m 644 PORTING CHANGES.gz $(README) $(PKG_DOC_DIR)
ifeq ($(PKG_DISTRIBUTION), debian)
	$(INSTALL) -S CHANGES.gz $(PKG_DOC_DIR)/changelog.gz
else
	$(INSTALL) -m 644 COPYING $(PKG_DOC_DIR)
endif

install-dev:
