/*
 * Copyright (c) 2000 Silicon Graphics, Inc.  All Rights Reserved.
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

#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include "types.h"
#include "fs.h"
#include "openutil.h"
#include "mlog.h"
#include "global.h"
#include "inventory.h"

#define VAR_PATH	"/var"
#define VARLIB_PATH	VAR_PATH"/lib"
#define XFSDUMP_PATH	VARLIB_PATH"/xfsdump"
#define VAR_MODE	0755
#define VAR_OWNER	0
#define VAR_GROUP	0

static char *paths[] = { VAR_PATH, VARLIB_PATH, XFSDUMP_PATH };
#define var_path	paths[0]
#define varlib_path	paths[1]
#define xfsdump_path	paths[2]

static void var_skip_recurse( char *, void ( * )( xfs_ino_t ));

void
var_create( void )
{
	intgen_t rval;
	intgen_t i;

	mlog( MLOG_DEBUG,
		"creating directory %s\n",
		xfsdump_path );

	for (i = 0 ; i < sizeof(paths)/sizeof(char *); i++) {
		rval = mkdir( paths[i], VAR_MODE );
		if ( rval && errno != EEXIST ) {
			mlog( MLOG_NORMAL,
			      "unable to create %s: %s\n",
			      paths[i],
			      strerror( errno ));
			return;
		}
		if ( rval == 0 ) {
			rval = chown( paths[i], VAR_OWNER, VAR_GROUP );
			if ( rval ) {
				mlog( MLOG_NORMAL,
				      "unable to chown %s: %s\n",
				      paths[i],
				      strerror( errno ));
			}
		}
	}
}


void
var_skip( uuid_t *dumped_fsidp, void ( *cb )( xfs_ino_t ino ))
{
	uuid_t fsid;
	intgen_t rval;

	/* see if the fs uuid's match
	 */
	rval = fs_getid( XFSDUMP_DIRPATH, &fsid );
	if ( rval ) {
#ifdef HIDDEN
                /* NOTE: this will happen for non-XFS file systems */
                /*       and is expected, so no msg */
		mlog( MLOG_NORMAL,
		      "unable to determine uuid of fs containing %s: "
		      "%s\n",
		      XFSDUMP_DIRPATH,
		      strerror( errno ));
#endif
		return;
	}

	if ( uuid_compare( *dumped_fsidp, fsid ) != 0) {
		return;
	}

	/* traverse the xfsdump directory, getting inode numbers of it
	 * and all of its children, and reporting those to the callback.
	 */
	var_skip_recurse( XFSDUMP_DIRPATH, cb );
}

static void
var_skip_recurse( char *base, void ( *cb )( xfs_ino_t ino ))
{
	struct stat64 statbuf;
	DIR *dirp;
	struct dirent *direntp;
	intgen_t rval;

	rval = lstat64( base, &statbuf );
	if ( rval ) {
		mlog( MLOG_NORMAL,
		      "unable to get status of %s: %s\n",
		      base,
		      strerror( errno ));
		return;
	}

	mlog( MLOG_DEBUG,
	      "excluding %s from dump\n",
	      base );

	( * cb )( statbuf.st_ino );

	if ( ( statbuf.st_mode & S_IFMT ) != S_IFDIR ) {
		return;
	}

	dirp = opendir( base );
	if ( ! dirp ) {
		mlog( MLOG_NORMAL,
		      "unable to open directory %s\n",
		      base );
		return;
	}

	while ( ( direntp = readdir( dirp )) != NULL ) {
		char *path;

		/* skip "." and ".."
		 */
		if ( *( direntp->d_name + 0 ) == '.'
		     &&
		     ( *( direntp->d_name + 1 ) == 0
		       ||
		       ( *( direntp->d_name + 1 ) == '.'
			 &&
			 *( direntp->d_name + 2 ) == 0 ))) {
			continue;
		}

		path = open_pathalloc( base, direntp->d_name, 0 );
		var_skip_recurse( path, cb );
		free( ( void * )path );
	}

	closedir( dirp );
}
