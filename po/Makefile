#
# Copyright (c) 2001-2002 Silicon Graphics, Inc.  All Rights Reserved.
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.
# 
# This program is distributed in the hope that it would be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# Further, this software is distributed without any warranty that it is
# free of the rightful claim of any third person regarding infringement
# or the like.  Any license provided herein, whether implied or
# otherwise, applies only to this software file.  Patent licenses, if
# any, provided herein do not apply to combinations of this program with
# other software, or any other product whatsoever.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston MA 02111-1307, USA.
# 
# Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
# Mountain View, CA  94043, or:
# 
# http://www.sgi.com 
# 
# For further information regarding this notice, see: 
# 
# http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
#

TOPDIR = ..
include $(TOPDIR)/include/builddefs

# Currently LINGUAS is undefined, so buildmacros provides no targets.
LINGUAS = 
LSRCFILES = $(LINGUAS:%=%.po) $(PKG_NAME).pot

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
		$(TOPDIR)/copy/xfs_copy.c \
		$(TOPDIR)/dump/content.c \
		$(TOPDIR)/dump/inomap.c \
		$(TOPDIR)/dump/var.c \
		$(TOPDIR)/estimate/xfs_estimate.c \
		$(TOPDIR)/fsr/xfs_fsr.c \
		$(TOPDIR)/inventory/inv_api.c \
		$(TOPDIR)/inventory/inv_core.c \
		$(TOPDIR)/inventory/inv_fstab.c \
		$(TOPDIR)/inventory/inv_mgr.c \
		$(TOPDIR)/inventory/inv_stobj.c \
		$(TOPDIR)/librmt/rmtioctl.c \
		$(TOPDIR)/librmt/rmtmsg.c \
		$(TOPDIR)/librmt/rmtopen.c \
		$(TOPDIR)/quota/xfsdq.c \
		$(TOPDIR)/restore/content.c \
		$(TOPDIR)/restore/dirattr.c \
		$(TOPDIR)/restore/inomap.c \
		$(TOPDIR)/restore/namreg.c \
		$(TOPDIR)/restore/node.c \
		$(TOPDIR)/restore/tree.c \
		$(TOPDIR)/restore/win.c

default: $(LINGUAS:%=%.mo)

include $(BUILDRULES)

install: default
	$(INSTALL_LINGUAS)

install-dev install-lib:
