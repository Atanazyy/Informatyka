#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "list.h"
#include "nand.h"

typedef struct pair {
    nand_t* gate;
    node_t* position;
} pair;

struct nand {
    bool is_evaluated;
    bool is_being_evaluated;
    bool value;
    int path_len;
    const bool* signal;
    int degree;
    int k;
    list_t* out;
    pair in[];
};

nand_t* nand_new(unsigned n) {
    nand_t* new_nand = malloc(sizeof(nand_t) + sizeof(pair) * n);
    if (new_nand == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    new_nand->out = init();
    if (new_nand->out == NULL) {
        free(new_nand);
        errno = ENOMEM;
        return NULL;
    }
    new_nand->is_evaluated = new_nand->is_being_evaluated = false;
    new_nand->signal = NULL;
    new_nand->path_len = 0;
    new_nand->k = n;
    for (int i = 0; i < (int)n; i++) {
        new_nand->in[i].gate = NULL;
        new_nand->in[i].position = NULL;
    }
    return new_nand;
};

void nand_delete(nand_t *g) {
    if (g == NULL) {
        return;
    }
    for (int i = 0; i < g->k; i++) {
        if(g->in[i].gate == NULL) {
            continue;
        }
        pop(g->in[i].gate->out, g->in[i].position);
        if(g->in[i].gate->signal != NULL) {
            nand_delete(g->in[i].gate);
        }
    }
    node_t *ptr = get_first(g->out);
    while (get_next(ptr) != get_last(g->out)) {
        ptr = get_next(ptr);
        get_nand(ptr)->in[get_k(ptr)].gate = NULL;
        get_nand(ptr)->in[get_k(ptr)].position = NULL;
    }
    destroy_list(g->out);
    free(g);
}

int nand_connect_nand(nand_t *g_out, nand_t *g_in, unsigned k) {
    if (g_in == NULL || g_out == NULL || (int)k >= g_in->k) {
        errno = EINVAL;
        return -1;
    }
    node_t* new_position = push(g_out->out, g_in, k);
    if (new_position == NULL) {
        errno = ENOMEM;
        return -1;
    }
    if (g_in->in[k].gate != NULL) {
        pop(g_in->in[k].gate->out, g_in->in[k].position);
    }
    g_in->in[k].position = new_position;
    g_in->in[k].gate = g_out;
    return 0;
}

int nand_connect_signal(bool const *s, nand_t *g, unsigned k) {
    if (s == NULL || g == NULL || (int)k >= g->k) {
        errno = EINVAL;
        return -1;
    }
    nand_t* new_nand = nand_new(0);
    if (new_nand == NULL) {
        errno = ENOMEM;
        return -1;
    }
    new_nand->signal = s;
    int ret = nand_connect_nand(new_nand, g, k);
    if(ret == -1) {
        nand_delete(new_nand);
    }
    return ret;
}

ssize_t nand_fan_out(nand_t const *g) {
    if (g == NULL) {
        errno = EINVAL;
        return -1;
    }
    return get_size(g->out);
}

void* nand_input(nand_t const *g, unsigned k) {
    if (g == NULL || (int)k >= g->k) {
        errno = EINVAL;
        return NULL;
    }
    if (g->in[k].gate == NULL) {
        return NULL;
    }
    if(g->in[k].gate->signal == NULL) {
        return g->in[k].gate;
    }
    return g->in[k].gate->signal;
}

nand_t* nand_output(nand_t const *g, ssize_t k) {
    node_t *ptr = get_first(g->out);
    for (int i = 0; i <= k; i++) {
        ptr = get_next(ptr);
    }
    return get_nand(ptr);
}

int max_int (int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

int evaluate_single(nand_t *g, list_t* all_gates) {
    if (g == NULL) {
        errno = ECANCELED;
        return -1;
    }
    if (g->signal != NULL) {
        g->value = *g->signal;
        g->path_len = 0;
        return 0;
    }
    if (g->is_evaluated) {
        return 0;
    }
    if(push(all_gates, g, -1) == NULL) {
        errno = ENOMEM;
        return -1;
    }
    if (g->is_being_evaluated) {
        errno = ECANCELED;
        return -1;
    }
    g->is_being_evaluated = true;
    g->value = false;
    g->path_len = 0;
    for (int i = 0; i < g->k; i++) {
        if (evaluate_single(g->in[i].gate, all_gates) == -1) {
            return -1;
        }
        if (g->in[i].gate->signal == NULL) {
            if (g->in[i].gate->value == false) {
                g->value = true;
            }
            g->path_len = max_int(g->path_len, g->in[i].gate->path_len + 1);
        }
        else {
            if(*(g->in[i].gate->signal) == false) {
                g->value = true;
            }
            g->path_len = max_int(g->path_len, 1);
        }
    }
    g->is_being_evaluated = false;
    g->is_evaluated = true;
    return 0;
}

void clear_info(list_t* all_gates) {
    node_t *ptr = get_first(all_gates);
    while (get_next(ptr) != get_last(all_gates)) {
        ptr = get_next(ptr);
        get_nand(ptr)->is_being_evaluated = false;
        get_nand(ptr)->is_evaluated = false;
    }
    destroy_list(all_gates);
}

ssize_t nand_evaluate(nand_t **g, bool *s, size_t m) {
    if(g == NULL || s == NULL || m == 0) {
        errno = EINVAL;
        return -1;
    }
    list_t* all_gates = init();
    if(all_gates == NULL) {
        errno = ENOMEM;
        return -1;
    }
    int max_path = 0;
    for (int i = 0; i < (int)m; i++) {
        if (g[i] == NULL) {
            errno = EINVAL;
            clear_info(all_gates);
            return -1;
        }
        if(evaluate_single(g[i], all_gates) == -1) {
            clear_info(all_gates);
            return -1;
        }
        s[i] = g[i]->value;
        max_path = max_int(max_path, g[i]->path_len);
    }
    clear_info(all_gates);
    return max_path;
}