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

#include <pthread.h>
#include <semaphore.h>

#include "types.h"
#include "qlock.h"
#include "mlog.h"

struct qlock {
	ix_t ql_ord;
		/* ordinal position of this lock
		 */
	pthread_mutex_t ql_mutex;
};

typedef struct  qlock qlock_t;
	/* internal qlock
	 */

#define QLOCK_THRDCNTMAX			256
	/* arbitrary limit on number of threads supported
	 */

static size_t qlock_thrdcnt;
	/* how many threads have checked in
	 */

typedef size_t ordmap_t;
	/* bitmap of ordinals. used to track what ordinals have
	 * been allocated.
	 */

#define ORDMAX					( 8 * sizeof( ordmap_t ))
	/* ordinals must fit into a wordsize bitmap
	 */

static ordmap_t qlock_ordalloced;
	/* to enforce allocation of only one lock to each ordinal value
	 */

struct thrddesc {
	pthread_t td_tid;
	ordmap_t td_ordmap;
};
typedef struct thrddesc thrddesc_t;
static thrddesc_t qlock_thrddesc[ QLOCK_THRDCNTMAX ];
	/* holds the ordmap for each thread
	 */

#define QLOCK_ORDMAP_SET( ordmap, ord )	( ordmap |= 1U << ord )
	/* sets the ordinal bit in an ordmap
	 */

#define QLOCK_ORDMAP_CLR( ordmap, ord )	( ordmap &= ~( 1U << ord ))
	/* clears the ordinal bit in an ordmap
	 */

#define QLOCK_ORDMAP_GET( ordmap, ord )	( ordmap & ( 1U << ord ))
	/* checks if ordinal set in ordmap
	 */

#define QLOCK_ORDMAP_CHK( ordmap, ord )	( ordmap & (( 1U << ord ) - 1U ))
	/* checks if any bits less than ord are set in the ordmap
	 */

/* REFERENCED */
static bool_t qlock_inited = BOOL_FALSE;
	/* to sanity check initialization
	 */

/* forward declarations
 */
static void qlock_ordmap_add( pthread_t tid );
static ordmap_t *qlock_ordmapp_get( pthread_t tid );
static ix_t qlock_thrdix_get( pthread_t tid );

bool_t
qlock_init( void )
{
	/* sanity checks
	 */
	ASSERT( ! qlock_inited );

	/* initially no threads checked in
	 */
	qlock_thrdcnt = 0;

	/* initially no ordinals allocated
	 */
	qlock_ordalloced = 0;

	/* now say we are initialized
	 */
	qlock_inited = BOOL_TRUE;

	/* add the parent thread to the thread list
	 */
	if ( ! qlock_thrdinit( )) {
		qlock_inited = BOOL_FALSE;
		return BOOL_FALSE;
	}

	return BOOL_TRUE;
}

bool_t
qlock_thrdinit( void )
{
	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* add thread to ordmap list
	 */
	qlock_ordmap_add( pthread_self() );

	return BOOL_TRUE;
}

qlockh_t
qlock_alloc( ix_t ord )
{
	qlock_t *qlockp;

	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* verify the ordinal is not already taken, and mark as taken
	 */
	ASSERT( ! QLOCK_ORDMAP_GET( qlock_ordalloced, ord ));
	QLOCK_ORDMAP_SET( qlock_ordalloced, ord );

	/* allocate lock memory
	 */
	qlockp = ( qlock_t * )calloc( 1, sizeof( qlock_t ));
	ASSERT( qlockp );

	/* initialize the mutex
	 */
	pthread_mutex_init( &qlockp->ql_mutex, NULL );

	/* assign the ordinal position
	 */
	qlockp->ql_ord = ord;

	return ( qlockh_t )qlockp;
}

void
qlock_lock( qlockh_t qlockh )
{
	qlock_t *qlockp = ( qlock_t * )qlockh;
	pthread_t tid;
	ix_t thrdix;
	ordmap_t *ordmapp;
	/* REFERENCED */
	intgen_t rval;
	
	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* get the caller's tid and thread index
	 */
	tid = pthread_self();

	thrdix = qlock_thrdix_get( tid );

	/* get the ordmap for this thread
	 */
	ordmapp = qlock_ordmapp_get( tid );

	/* assert that this lock not already held by this thread
	 */
	if ( QLOCK_ORDMAP_GET( *ordmapp, qlockp->ql_ord )) {
		mlog( MLOG_NORMAL | MLOG_WARNING | MLOG_NOLOCK,
		      _("lock already held: thrd %d tid %lu ord %d map %x\n"),
		      thrdix,
		      tid,
		      qlockp->ql_ord,
		      *ordmapp );
	}
	ASSERT( ! QLOCK_ORDMAP_GET( *ordmapp, qlockp->ql_ord ));

	/* assert that no locks with a lesser ordinal are held by this thread
	 */
	if ( QLOCK_ORDMAP_CHK( *ordmapp, qlockp->ql_ord )) {
		mlog( MLOG_NORMAL | MLOG_WARNING | MLOG_NOLOCK,
		      _("lock ordinal violation: thrd %d tid %lu ord %d map %x\n"),
		      thrdix,
		      tid,
		      qlockp->ql_ord,
		      *ordmapp );
	}
	ASSERT( ! QLOCK_ORDMAP_CHK( *ordmapp, qlockp->ql_ord ));

	/* acquire the lock
	 */
	rval = pthread_mutex_lock( &qlockp->ql_mutex );
	ASSERT( !rval );

	/* add ordinal to this threads ordmap
	 */
	QLOCK_ORDMAP_SET( *ordmapp, qlockp->ql_ord );
}

void
qlock_unlock( qlockh_t qlockh )
{
	qlock_t *qlockp = ( qlock_t * )qlockh;
	ordmap_t *ordmapp;
	/* REFERENCED */
	intgen_t rval;
	
	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* get the ordmap for this thread
	 */
	ordmapp = qlock_ordmapp_get( pthread_self() );

	/* verify lock is held by this thread
	 */
	ASSERT( QLOCK_ORDMAP_GET( *ordmapp, qlockp->ql_ord ));

	/* clear lock's ord from thread's ord map
	 */
	QLOCK_ORDMAP_CLR( *ordmapp, qlockp->ql_ord );
	
	/* release the lock
	 */
	rval = pthread_mutex_unlock( &qlockp->ql_mutex );
	ASSERT( ! rval );
}

qsemh_t
qsem_alloc( ix_t cnt )
{
	sem_t *semp;
	intgen_t rval;

	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* allocate a semaphore
	 */
	semp = ( sem_t * )calloc( 1, sizeof( sem_t ));
	ASSERT( semp );

	/* initialize the semaphore
	 */
	rval = sem_init( semp, 0, cnt );
	ASSERT( !rval );

	return ( qsemh_t )semp;
}

void
qsem_free( qsemh_t qsemh )
{
	sem_t *semp = ( sem_t * )qsemh;
	intgen_t rval;

	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* destroy the mutex and condition
	 */
	rval = sem_destroy( semp );
	ASSERT( !rval );

	/* free the semaphore
	 */
	free( semp );
}

void
qsemP( qsemh_t qsemh )
{
	sem_t *semp = ( sem_t * )qsemh;
	intgen_t rval;
	
	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* "P" the semaphore
	 */
	rval = sem_wait( semp );
	ASSERT( !rval );
}

void
qsemV( qsemh_t qsemh )
{
	sem_t *semp = ( sem_t * )qsemh;
	intgen_t rval;
	
	/* sanity checks
	 */
	ASSERT( qlock_inited );

	/* "V" the semaphore
	 */
	rval = sem_post( semp );
	ASSERT( !rval );
}

bool_t
qsemPwouldblock( qsemh_t qsemh )
{
	sem_t *semp = ( sem_t * )qsemh;
	int count;
	intgen_t rval;

	/* sanity checks
	 */
	ASSERT( qlock_inited );

	rval = sem_getvalue( semp, &count );
	ASSERT( !rval );

	return count <= 0 ? BOOL_TRUE : BOOL_FALSE;
}

size_t
qsemPavail( qsemh_t qsemh )
{
	sem_t *semp = ( sem_t * )qsemh;
	int count;
	intgen_t rval;

	/* sanity checks
	 */
	ASSERT( qlock_inited );

	rval = sem_getvalue( semp, &count );
	ASSERT( !rval );

	return count < 0 ? 0 : count;
}

/* internal ordinal map abstraction
 */
static void
qlock_ordmap_add( pthread_t tid )
{
	ASSERT( qlock_thrdcnt < QLOCK_THRDCNTMAX );
	qlock_thrddesc[ qlock_thrdcnt ].td_tid = tid;
	qlock_thrddesc[ qlock_thrdcnt ].td_ordmap = 0;
	qlock_thrdcnt++;
}

static thrddesc_t *
qlock_thrddesc_get( pthread_t tid )
{
	thrddesc_t *p;
	thrddesc_t *endp;

	for ( p = &qlock_thrddesc[ 0 ],
	      endp = &qlock_thrddesc[ qlock_thrdcnt ]
	      ;
	      p < endp
	      ;
	      p++ ) {
		if ( pthread_equal( p->td_tid, tid ) ) {
			return p;
		}
	}

	return 0;
}

static ordmap_t *
qlock_ordmapp_get( pthread_t tid )
{
	thrddesc_t *p;
	p = qlock_thrddesc_get( tid );
	return &p->td_ordmap;
}

static ix_t
qlock_thrdix_get( pthread_t tid )
{
	thrddesc_t *p;
	p = qlock_thrddesc_get( tid );
	ASSERT( p >= &qlock_thrddesc[ 0 ] );
	return ( ix_t )( p - &qlock_thrddesc[ 0 ] );
}
