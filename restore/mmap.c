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

#include <xfs/libxfs.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

/*
 * our custom mmap b/c Linux doesn't have MAP_AUTOGROW
 * and for an empty file we need to write to last byte
 * to ensure it can be accessed
 */

void *
mmap_autogrow(size_t len, int fd, off_t offset)
{
    struct stat buf;
    char nul_buffer[] = "";

    /* prealloc file if it is an empty file */
    if (fstat(fd, &buf) == -1) {;
	return (void*)MAP_FAILED;
    }
    if (buf.st_size < offset+len) {
	(void)lseek(fd, offset+len-1, SEEK_SET);
	(void)write(fd, nul_buffer, 1);
    }

    return mmap( 0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset );
}
