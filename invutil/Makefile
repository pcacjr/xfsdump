#
# Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
#

TOPDIR = ..
include $(TOPDIR)/include/builddefs

COMMINCL = \
	global.h \
	mlog.h \
	timeutil.h \
	types.h

INVINCL = \
	inventory.h \
	inv_priv.h

INVCOMMON = \
	inv_files.c

COMMON = \
	timeutil.c

CURSESCOMMON = \
	cmenu.c \
	fstab.c \
	invidx.c \
	list.c \
	menu.c \
	screen.c \
	stobj.c

CURSESINCL = \
	cmenu.h \
	fstab.h \
	getopt.h \
	invidx.h \
	invutil.h \
	list.h \
	stobj.h

LOCALS = invutil.c

LTCOMMAND = xfsinvutil
CFILES = $(LOCALS)
LCFILES = $(COMMON) $(INVCOMMON)
LHFILES = $(COMMINCL) $(INVINCL)
LINKS  = $(COMMINCL) $(COMMON) $(INVINCL) $(INVCOMMON)
LDIRT = $(LINKS)
LLDLIBS = $(LIBUUID) $(LIBCURSES)

#LCFLAGS = -DDUMP -DRMT -DEXTATTR -DBASED -DDOSOCKS -DINVCONVFIX -DSIZEEST -DPIPEINVFIX
LCFLAGS = -DDUMP -DBASED -DDOSOCKS -DINVCONVFIX -DSIZEEST -DPIPEINVFIX
#LCFLAGS = -DINV_DEBUG -DDUMP -DBASED -DDOSOCKS -DINVCONVFIX -DSIZEEST -DPIPEINVFIX

ifeq ($(ENABLE_CURSES), yes)
CFILES += $(CURSESCOMMON)
HFILES += $(CURSESINCL)
LCFLAGS += -DHAVE_CURSES
else
LSRCFILES += $(CURSESCOMMON) $(CURSESINCL)
endif

default: $(LTCOMMAND)

include $(BUILDRULES)

install: default
	$(INSTALL) -m 755 -d $(PKG_SBIN_DIR)
	$(LTINSTALL) -m 755 $(LTCOMMAND) $(PKG_SBIN_DIR)
install-dev:

$(COMMINCL) $(COMMON):
	$(RM) $@; $(LN_S) ../common/$@ $@

$(INVINCL) $(INVCOMMON):
	$(RM) $@; $(LN_S) ../inventory/$@ $@
