/*
 * Copyright (c) 2002 Silicon Graphics, Inc.  All Rights Reserved.
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
#ifndef STOBJ_H
#define STOBJ_H

#include "types.h"
#include "mlog.h"
#include "inv_priv.h"
#include "list.h"
#include "cmenu.h"

node_t *generate_stobj_menu(node_t *startnode, int level, char *StObjFileName);
int open_stobj(char *StObjFileName);
int close_stobj_file(int fidx, int unlink_ok);
int close_all_stobj(void);
int stobjsess_highlight(WINDOW *win, node_t *current, node_t *list);
int stobjstrm_highlight(WINDOW *win, node_t *current, node_t *list);
int stobjmed_highlight(WINDOW *win, node_t *current, node_t *list);
int stobj_select(WINDOW *win, node_t *current, node_t *list);
int stobjsess_commit(WINDOW *win, node_t *current, node_t *list);
int stobj_prune(char *mountpt, uuid_t *uuidp, time_t prunetime, node_t *node, node_t *list);
int stobj_undelete(WINDOW *win, node_t *current, node_t *list);
int stobjmed_commit(WINDOW *win, node_t *current, node_t *list);
int stobjstrm_commit(WINDOW *win, node_t *current, node_t *list);
int stobjstrm_delete(WINDOW *win, node_t *current, node_t *list);
int stobjmed_delete(WINDOW *win, node_t *current, node_t *list);

#endif /* STOBJ_H */
