#ifndef LIST_H
#define LIST_H

struct menu_ops_s;

typedef struct node_s {
    void *data;
    struct node_s *next;
    struct node_s *prev;
} node_t;

typedef struct {
    int		level;		/* indentation level in menu */
    int		hidden;		/* flag: displayed in menu */
    int		expanded;	/* flag: children displayed in menu */
    int		deleted;	/* flag: marked as deleted in menu */
    int		imported;	/* flag: marked as imported in menu */
    int		commited;	/* flag: record already commited */
    int		file_idx;	/* index into file table */
    char	*text;		/* text displayed in menu */
    struct menu_ops_s *ops;	/* table of applicable operations */
    node_t	*parent;	/* parent node */
    node_t	**children;	/* child nodes */
    int		nbr_children;	/* number of children nodes */
    int		data_idx;	/* real data in mmap file */
} data_t;

void node_free(node_t *node);
node_t *node_create(int hidden, int expanded, int level, int deleted, int file_idx, char *text,
		    struct menu_ops_s *ops, node_t *parent, node_t **children, int nbr_children,
		    int data_idx);
node_t *list_add(node_t *prevnode, node_t *newnode);
node_t *list_del(node_t *node);
node_t *free_all_children(node_t *node);
node_t *mark_all_children_commited(node_t *node);

#endif /* LIST_H */

