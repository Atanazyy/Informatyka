#ifndef LIST_H
#define LIST_H

#include "nand.h"

typedef struct list list_t;
typedef struct node node_t;

list_t *init();
node_t *push(list_t *l, nand_t *nand, int k);
void pop(list_t *l, node_t *removed_node);
node_t *get_first(list_t *l);
node_t *get_last(list_t *l);
node_t *get_next(node_t *n);
nand_t *get_nand(node_t *n);
int get_k(node_t *n);
int get_size(list_t *n);
void destroy_list(list_t *l);
#endif
