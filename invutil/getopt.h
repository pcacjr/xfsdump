/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 */

#ident "$Header: /home/cattelan/osspush/CVSROOT/xfs-cmds/xfsdump/invutil/getopt.h,v 1.2 2002/07/30 01:24:34 nathans Exp $"

#ifndef GETOPT_H
#define GETOPT_H

#define GETOPT_CMDSTRING	"dilnu:wCFM:"

#define GETOPT_DEBUG		'd'	/* debug */
#define GETOPT_INTERACTIVE	'i'	/* interactive mode */
#define GETOPT_NONINTERACTIVE	'n'	/* non interactive mode (default) */
#define GETOPT_UUID		'u'	/* prune uuid */
#define GETOPT_WAITFORLOCKS	'w'	/* wait for locks */
#define GETOPT_CHECKPRUNEFSTAB	'C'	/* check and prune fstab */
#define GETOPT_FORCE		'F'	/* force - do not ask for confirmation */
#define GETOPT_PRUNEMNT		'M'	/* prune mount point */

#endif /* GETOPT_H */
