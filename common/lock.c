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

#include <xfs/xfs.h>
#include <xfs/jdm.h>

#include "types.h"
#include "qlock.h"
#include "mlog.h"

static qlockh_t lock_qlockh = QLOCKH_NULL;

bool_t
lock_init( void )
{
	/* initialization sanity checks
	 */
	ASSERT( lock_qlockh == QLOCKH_NULL );

	/* allocate a qlock
	 */
	lock_qlockh = qlock_alloc( QLOCK_ORD_CRIT );

	return BOOL_TRUE;
}

void
lock( void )
{
	qlock_lock( lock_qlockh );
}

void
unlock( void )
{
	qlock_unlock( lock_qlockh );
}
