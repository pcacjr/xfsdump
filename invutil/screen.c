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

#include <xfs/libxfs.h>
#include <xfs/jdm.h>
#include <ncurses.h>

#include "cmenu.h"

int
put_line(WINDOW *win, int line, char *msg, int attr, alignment_t alignment)
{
    int c;
    int cols;
    int lines;
    int msglen;
    static char text[256];

    getmaxyx(win, lines, cols);
    cols--;
    if(cols > 255) {
	cols = 255;
    }
    msglen = strlen(msg);
    if(msglen > cols)
	msglen = cols;

    switch(alignment) {
    case ALIGN_CENTRE:
	c = (cols - msglen) / 2;
	break;
    case ALIGN_RIGHT:
	c = (cols - msglen);
	break;
    case ALIGN_LEFT:
    default:
	c = 0;
	break;
    }

    snprintf(text, sizeof(text), "%*s%.*s%*s", c, "", msglen, msg, cols - msglen - c, "");

    wmove(win, line, 0);
    wattron(win, attr);
    waddstr(win, text);
    wattroff(win, attr);

    return 0;
}

void
hitanykey()
{
    put_footer("Hit any key to continue...", ALIGN_CENTRE);
    wgetch(stdscr);
    clear_line(stdscr, LINES - 1);
}

/*ARGSUSED*/
int
get_string(WINDOW *win, char *prompt, char *response, int len)
{
    put_line(stdscr, LINES - 1, prompt, 0, ALIGN_LEFT);
    wmove(stdscr, LINES - 1, strlen(prompt));
    echo();
    wgetnstr(stdscr, response, len);
    noecho();
    
    return 0;
}
