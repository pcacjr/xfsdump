#include <libxfs.h>
#include <jdm.h>
#include <curses.h>

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

    sprintf(text, "%*s%.*s%*s", c, "", msglen, msg, cols - msglen - c, "");

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
