/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
 */

#ident "$Header: /home/cattelan/osspush/CVSROOT/xfs-cmds/xfsdump/invutil/getopt.h,v 1.1 2002/07/29 06:04:12 nathans Exp $"

#ifndef GETOPT_H
#define GETOPT_H

#define GETOPT_CMDSTRING	"dilu:wCFM:"

#define GETOPT_DEBUG		'd'	/* debug */
#define GETOPT_INTERACTIVE	'i'	/* interactive mode */
#define GETOPT_UUID		'u'	/* prune uuid */
#define GETOPT_WAITFORLOCKS	'w'	/* wait for locks */
#define GETOPT_CHECKPRUNEFSTAB	'C'	/* check and prune fstab */
#define GETOPT_FORCE		'F'	/* force - do not ask for confirmation */
#define GETOPT_PRUNEMNT		'M'	/* prune mount point */

#endif /* GETOPT_H */
