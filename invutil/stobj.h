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
