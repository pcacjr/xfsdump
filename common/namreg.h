/*
 * Copyright (c) 2000-2001 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef NAMREG_H
#define NAMREG_H

/* namreg.[hc] - directory entry registry
 *
 * provides unique directory entry ID's and storage for the entry
 * name.
 */

/* namreg_t - external handle; internally typecast to something more useful
 */
typedef void namreg_t;


/* namreg_ix_t - abstract index of a registered name
 */
typedef size_t namreg_ix_t;
#define NAMREG_IX_NULL	( ( namreg_ix_t )( -1 ))
#define NAMREG_IX_MAX	SIZEMAX


/* namreg_init - creates a name registry. returns namreg_t pointer
 */
extern namreg_t *namreg_init( bool_t cumulative,
			      bool_t delta,
			      char *housekeepingdir );


/* namreg_add - registers a name. name does not need to be null-terminated.
 * returns abstract index for use with namreg_get().
 */
extern namreg_ix_t namreg_add( namreg_t *namregp, char *name, size_t namelen );


/* namreg_del - remove a name from the registry
 */
extern void namreg_del( namreg_t *namregp, namreg_ix_t namreg_ix );


/* namreg_get - retrieves the name identified by the index.
 * fills the buffer with the null-terminated name from the registry.
 * returns the strlen() of the name. returns -1 if the buffer is too
 * small to fit the null-terminated name. return -2 if the name
 * not in the registry. return -3 if a system call fails.
 */
extern intgen_t namreg_get( namreg_t *namregp,
			    namreg_ix_t namreg_ix,
			    char *bufp,
			    size_t bufsz );


#endif /* NAMREG_H */
