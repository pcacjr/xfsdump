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

#include <stdlib.h>
#include <signal.h>
#include <sched.h>

#define STACKSIZE 65536

int
sproc (int (*entry) (void *), int flags, void *arg)
{
    int retval = -1;
#ifdef HIDDEN
    void *newstack;

    if ( (newstack = calloc (1, STACKSIZE)) != NULL ) {
        void ** stackp = ((void **)newstack)+(STACKSIZE -1)/sizeof(void*);

        flags |= SIGCHLD;

        retval = clone (entry, stackp, flags, arg);
    }
#endif

    return retval;
}
