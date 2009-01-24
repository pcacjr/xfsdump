#
# Copyright (c) 2001-2002 Silicon Graphics, Inc.  All Rights Reserved.
#

TOPDIR = ..
include $(TOPDIR)/include/builddefs

POTHEAD = $(PKG_NAME).pot
LINGUAS =
LSRCFILES = $(LINGUAS:%=%.po) $(POTHEAD)
LDIRT = $(POTHEAD)

# TODO - invutil/
XGETTEXTFILES =	\
		$(TOPDIR)/common/cldmgr.c \
		$(TOPDIR)/common/content_common.c \
		$(TOPDIR)/common/dlog.c \
		$(TOPDIR)/common/drive.c \
		$(TOPDIR)/common/drive_minrmt.c \
		$(TOPDIR)/common/drive_scsitape.c \
		$(TOPDIR)/common/drive_simple.c \
		$(TOPDIR)/common/fs.c \
		$(TOPDIR)/common/global.c \
		$(TOPDIR)/common/main.c \
		$(TOPDIR)/common/media.c \
		$(TOPDIR)/common/mlog.c \
		$(TOPDIR)/common/namreg.c \
		$(TOPDIR)/common/openutil.c \
		$(TOPDIR)/common/qlock.c \
		$(TOPDIR)/common/util.c \
		$(TOPDIR)/dump/content.c \
		$(TOPDIR)/dump/inomap.c \
		$(TOPDIR)/dump/var.c \
		$(TOPDIR)/inventory/inv_api.c \
		$(TOPDIR)/inventory/inv_core.c \
		$(TOPDIR)/inventory/inv_fstab.c \
		$(TOPDIR)/inventory/inv_mgr.c \
		$(TOPDIR)/inventory/inv_stobj.c \
		$(TOPDIR)/librmt/rmtioctl.c \
		$(TOPDIR)/librmt/rmtmsg.c \
		$(TOPDIR)/librmt/rmtopen.c \
		$(TOPDIR)/restore/content.c \
		$(TOPDIR)/restore/dirattr.c \
		$(TOPDIR)/restore/inomap.c \
		$(TOPDIR)/restore/namreg.c \
		$(TOPDIR)/restore/node.c \
		$(TOPDIR)/restore/tree.c \
		$(TOPDIR)/restore/win.c

default: $(POTHEAD) $(LINGUAS:%=%.mo)

include $(BUILDRULES)

install: default
	$(INSTALL_LINGUAS)

install-dev install-lib:
