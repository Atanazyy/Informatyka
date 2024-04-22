#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <stdio.h>
#include "nand.h"
#include "list.h"

struct node
{
    node_t *prev;
    node_t *next;
    nand_t *nand;
    unsigned k; // to wich input gate is connected
};

struct list
{
    unsigned size;
    node_t *first;
    node_t *last;
};

// create new list
list_t *init()
{
    list_t *new_list = malloc(sizeof(list_t));
    node_t *first = malloc(sizeof(node_t));
    node_t *last = malloc(sizeof(node_t));
    if (new_list == NULL || first == NULL || last == NULL)
    {
        free(new_list);
        free(first);
        free(last);
        return NULL;
    }
    first->prev = last->next = NULL;
    first->k = last->k = -1;
    first->next = last;
    last->prev = first;
    new_list->first = first;
    new_list->last = last;
    new_list->size = 0;
    return new_list;
}

// add element to the list
node_t *push(list_t *l, nand_t *nand, unsigned k)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        return NULL;
    }
    new_node->next = l->last;
    new_node->prev = l->last->prev;
    l->last->prev->next = new_node;
    l->size++;
    l->last->prev = new_node;
    new_node->nand = nand;
    new_node->k = k;
    return new_node;
}

// delete element from the list
void pop(list_t *l, node_t *removed_node)
{
    if (l == NULL || removed_node == NULL || l->size == 0)
    {
        return;
    }
    removed_node->prev->next = removed_node->next;
    removed_node->next->prev = removed_node->prev;
    l->size--;
    free(removed_node);
}

static void destroy_node(node_t *n)
{
    if (n == NULL)
    {
        return;
    }
    destroy_node(n->next);
    free(n);
}

// delete list and free the memory
void destroy_list(list_t *l)
{
    destroy_node(l->first);
    free(l);
}

node_t *get_first(list_t *l)
{
    return l->first;
}

node_t *get_last(list_t *l)
{
    return l->last;
}

node_t *get_next(node_t *n)
{
    return n->next;
}

nand_t *get_nand(node_t *n)
{
    return n->nand;
}

unsigned get_k(node_t *n)
{
    return n->k;
}

unsigned get_size(list_t *l)
{
    return l->size;
}