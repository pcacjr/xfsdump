#ifndef FSTAB_H
#define FSTAB_H

#include "types.h"
#include "mlog.h"
#include "inv_priv.h"
#include "list.h"
#include "cmenu.h"

/* fstab.c */
node_t *generate_fstab_menu(char *inv_path, node_t *startnode, int level, char *fstabname);
int open_fstab(char *fstabname);
int close_all_fstab(void);
void *remmap_fstab(int fidx, int num);
int find_matching_fstab(int fidx, invt_fstab_t *fstabentry);
int fstab_select(WINDOW *win, node_t *current, node_t *list);
int fstab_highlight(WINDOW *win, node_t *current, node_t *list);
int fstab_commit(WINDOW *win, node_t *current, node_t *list);
int fstab_prune(char *mountpt, uuid_t *uuidp, time_t prunetime, node_t *node, node_t *list);

#endif /* FSTAB_H */
