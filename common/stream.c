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

#include "types.h"
#include "exit.h"
#include "stream.h"
#include "lock.h"
#include "mlog.h"

#define PROCMAX	( STREAM_MAX * 2 + 1 )
#define N(a) (sizeof((a)) / sizeof((a)[0]))

struct spm {
	stream_state_t	s_state;
	pid_t		s_pid;
	intgen_t	s_ix;
	int		s_exit_code;
	rv_t		s_exit_return;
	rv_t		s_exit_hint;
};

typedef struct spm spm_t;
extern pid_t parentpid;
static spm_t spm[ STREAM_MAX * 3 ];

void
stream_init( void )
{
#ifdef HIDDEN
	/* REFERENCED */
	intgen_t rval;

	rval = ( intgen_t )usconfig( CONF_INITUSERS, PROCMAX );
	ASSERT( rval >= 0 );
#endif /* HIDDEN */

	( void )memset( ( void * )spm, 0, sizeof( spm ));

}

/*
 * Note that the stream list structure (updated via the stream_* functions)
 * is indexed by pid. Multiple processes can be registered against the same
 * stream index, typically: the primary content process that does the work;
 * and the drive slave process, which just processes stuff off the ring buffer.
 * In general having multiple pids registered per stream is not an issue for
 * termination status reporting, as the mlog_exit* logging functions only
 * ever get called out of the primary content process.
 */

void
stream_register( pid_t pid, intgen_t streamix )
{
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);

	ASSERT( streamix < STREAM_MAX );

	lock();
	for ( ; p < ep ; p++ ) {
		if ( p->s_state == S_FREE ) {
			p->s_state = S_RUNNING;
			break;
		}
	}
	unlock();
	ASSERT( p < ep );

	if ( p >= ep ) return;

	p->s_pid = pid;
	p->s_ix = streamix;
	p->s_exit_code = -1;
	p->s_exit_return = RV_NONE;
	p->s_exit_hint = RV_NONE;
}

void
stream_dead( pid_t pid )
{
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);

	lock();
	for ( ; p < ep ; p++ )
		if ( p->s_pid == pid ) {
			p->s_state = S_ZOMBIE;
			break;
		}
	unlock();
	ASSERT( p < ep );
}

void
stream_free( pid_t pid )
{
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);

	lock();
	for ( ; p < ep ; p++ ) {
		if ( p->s_pid == pid ) {
			(void) memset( (void *) p, 0, sizeof(spm_t) );
			p->s_state = S_FREE;
			break;
		}
	}
	unlock();
	ASSERT( p < ep );
}

int
stream_find_all( stream_state_t states[], int nstates,
		 pid_t pids[], int npids )
{
	int i, count = 0;
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);
	ASSERT(nstates > 0 && npids > 0);

	/* lock - make sure we get a consistent snapshot of the stream status */
	lock();
	for ( ; p < ep && count < npids; p++ )
		for (i = 0; i < nstates; i++)
			if (p->s_state == states[i]) {
				pids[count++] = p->s_pid;
				break;
			}
	unlock();
	return count;
}

static spm_t *
stream_find( pid_t pid, stream_state_t s[], int nstates )
{
	int i;
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);
	static const char *state_strings[] = { "S_FREE", "S_RUNNING", "S_ZOMBIE" };

	ASSERT(nstates > 0);

	/* note we don't lock the stream array in this function */
	for ( ; p < ep ; p++ )
		if ( p->s_pid == pid ) {
			/* check state */
			for (i = 0; i < nstates; i++)
				if (p->s_state == s[i])
					return p;
		}

	mlog( MLOG_DEBUG | MLOG_ERROR | MLOG_NOLOCK | MLOG_BARE,
	      "stream_find(): no stream with pid: %d and state%s:",
	      pid, nstates == 1 ? "" : "s" );
	for (i = 0; i < nstates; i++)
		mlog( MLOG_DEBUG | MLOG_ERROR | MLOG_NOLOCK | MLOG_BARE,
		      " %s", state_strings[s[i]]);
	mlog( MLOG_DEBUG | MLOG_ERROR | MLOG_NOLOCK | MLOG_BARE, "\n");

	return (spm_t *) NULL;
}

intgen_t
stream_getix( pid_t pid )
{
	stream_state_t states[] = { S_RUNNING };
	spm_t *p;
	intgen_t ix;
	lock();
	p = stream_find( pid, states, N(states) );
	ix = p ? p->s_ix : -1;
	unlock();
	return ix;
}


/*
 * We don't currently export spm_t and don't really want callers
 * keeping pointers into the stream array, so use explicit access
 * functions. Note we only expect these to be called from running/owner
 * streams.
 */

#define stream_set(field_name, pid, value)				\
	stream_state_t states[] = { S_RUNNING };			\
	spm_t *p;							\
	pid_t mypid = getpid();						\
									\
	if (mypid != (pid)) {						\
		mlog( MLOG_DEBUG | MLOG_ERROR | MLOG_NOLOCK | MLOG_BARE,\
		      "stream_set_" #field_name "(): "			\
		      "foreign stream (pid %d) "			\
		      "not permitted to update this stream (pid %d)\n",	\
		      mypid, (pid));					\
		return;							\
	}								\
									\
	lock();								\
	p = stream_find( (pid), states, N(states) );			\
	if (p) p->s_exit_ ## field_name = (value);			\
	unlock();

void stream_set_code( pid_t pid, int exit_code )
{
	stream_set( code, pid, exit_code );
}

void stream_set_return( pid_t pid, rv_t rv )
{
	stream_set( return, pid, rv );
}

void stream_set_hint( pid_t pid, rv_t rv )
{
	stream_set( hint, pid, rv );
}


bool_t
stream_get_exit_status( pid_t pid,
			stream_state_t states[],
			int nstates,
			stream_state_t *state,
			intgen_t *ix,
			int *exit_code,
			rv_t *exit_return,
			rv_t *exit_hint)
{
	bool_t sts = BOOL_FALSE;
	spm_t *p;

	lock();
	p = stream_find( pid, states, nstates );
	if (! p) goto unlock;

	if (state) *state = p->s_state;
	if (ix) *ix = p->s_ix;
	if (exit_code) *exit_code = p->s_exit_code;
	if (exit_return) *exit_return = p->s_exit_return;
	if (exit_hint) *exit_hint = p->s_exit_hint;
	sts = BOOL_TRUE;

 unlock:
	unlock();
	return sts;
}

size_t
stream_cnt( void )
{
	spm_t *p = spm;
	spm_t *ep = spm + N(spm);
	size_t ixmap = 0;
	size_t ixcnt;
	size_t bitix;

	ASSERT( sizeof( ixmap ) * NBBY >= STREAM_MAX );
	
	lock();
	for ( ; p < ep ; p++ ) {
		if ( p->s_state == S_RUNNING ) {
			ixmap |= ( size_t )1 << p->s_ix;
		}
	}
	unlock();

	ixcnt = 0;
	for ( bitix = 0 ; bitix < STREAM_MAX ; bitix++ ) {
		if ( ixmap & ( ( size_t )1 << bitix )) {
			ixcnt++;
		}
	}

	return ixcnt;
}
