/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.  All Rights Reserved.
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * Further, this software is distributed without any warranty that it is
 * free of the rightful claim of any third person regarding infringement
 * or the like.  Any license provided herein, whether implied or
 * otherwise, applies only to this software file.  Patent licenses, if
 * any, provided herein do not apply to combinations of this program with
 * other software, or any other product whatsoever.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc., 59
 * Temple Place - Suite 330, Boston MA 02111-1307, USA.
 * 
 * Contact information: Silicon Graphics, Inc., 1600 Amphitheatre Pkwy,
 * Mountain View, CA  94043, or:
 * 
 * http://www.sgi.com 
 * 
 * For further information regarding this notice, see: 
 * 
 * http://oss.sgi.com/projects/GenInfo/SGIGPLNoticeExplan/
 */

#include <libxfs.h>
#include <jdm.h>

#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/dir.h>


/*----------------------------------------------------------------------*/
/*                                                                      */
/* Functions which access inventory paths: /var/[lib/]xfsdump/inventory */
/* The [lib] component was introduced for FHS compliance on Linux.      */
/*                                                                      */
/*----------------------------------------------------------------------*/

#define MGR_BASE_NEW	"/var/lib/xfsdump"
#define MGR_BASE_OLD	"/var/xfsdump"
#define MGR_DIRPATH	"/inventory"		/* preceed with inv_base */
#define MGR_FSTAB	MGR_DIRPATH"/fstab"	/* preceed with inv_base */
#define MGR_LOCKFILE	MGR_DIRPATH"/invlock"	/* preceed with inv_base */
#define MGR_PATH_MAX	64	/* PATH_MAX would be a tad wasteful here */

static char *inv_base;
static char new_inv_base[] = MGR_BASE_NEW;
static char old_inv_base[] = MGR_BASE_OLD;

static char inv_fstabp[MGR_PATH_MAX];
static char inv_dirpathp[MGR_PATH_MAX];
static char inv_lockfilep[MGR_PATH_MAX];	

char *
inv_dirpath( void )
{
	ASSERT(inv_base);
	return inv_dirpathp;
}

char *
inv_fstab( void )
{
	ASSERT(inv_base);
	return inv_fstabp;
}

char *
inv_lockfile( void )
{
	ASSERT(inv_base);
	return inv_lockfilep;
}

char *
inv_basepath( void )
{
	ASSERT(inv_base);
	return inv_base;
}

int
inv_setup_base( void )
{
	struct stat64 new_sbuf, old_sbuf;
	int new_exists, old_exists;

	errno = 0;
	new_exists = !(stat64(new_inv_base, &new_sbuf) < 0 && errno == ENOENT);
	errno = 0;
	old_exists = !(stat64(old_inv_base, &old_sbuf) < 0 && errno == ENOENT);

	if (new_exists && old_exists) {
		if (new_sbuf.st_ino == old_sbuf.st_ino &&
		    new_sbuf.st_dev == old_sbuf.st_dev) {
			old_exists = 0;	/* both the same, pretend */
		}
		else {	/* die! */
			return 0;
		}
	}

	if (!old_exists)
		inv_base = new_inv_base;
	else
		inv_base = old_inv_base;

	/* also setup the dependent paths now
	 */
	strcpy(inv_dirpathp, inv_base);
	strcat(inv_dirpathp, MGR_DIRPATH);

	strcpy(inv_fstabp, inv_base);
	strcat(inv_fstabp, MGR_FSTAB);

	strcpy(inv_lockfilep, inv_base);
	strcat(inv_lockfilep, MGR_LOCKFILE);

	return 1;
}
