#include "list.h"
#include <stdlib.h>

typedef struct _Node {
    const void *data;
    struct _Node *next;
} Node;

struct _List {
    Node *first;
    Node *last;
    int size;
};

static Node *node_new(const void *e) {
    Node *n;
    if (!e) return NULL;
    n = (Node *)malloc(sizeof(Node));
    if (!n) return NULL;
    n->data = e;
    n->next = NULL;
    return n;
}

List *list_new() {
    List *pl = (List *)malloc(sizeof(List));
    if (!pl) return NULL;
    pl->first = NULL;
    pl->last = NULL;
    pl->size = 0;
    return pl;
}

Bool list_isEmpty(const List *pl) {
    if (!pl || pl->size == 0) return TRUE;
    return FALSE;
}

Status list_pushFront(List *pl, const void *e) {
    Node *n;
    if (!pl || !e) return ERROR;
    n = node_new(e);
    if (!n) return ERROR;
    n->next = pl->first;
    pl->first = n;
    if (!pl->last) pl->last = n;
    pl->size++;
    return OK;
}

Status list_pushBack(List *pl, const void *e) {
    Node *n;
    if (!pl || !e) return ERROR;
    n = node_new(e);
    if (!n) return ERROR;
    if (list_isEmpty(pl) == TRUE) {
        pl->first = n;
        pl->last = n;
    } else {
        pl->last->next = n;
        pl->last = n;
    }
    pl->size++;
    return OK;
}

void *list_popFront(List *pl) {
    Node *n; void *data;
    if (!pl || list_isEmpty(pl) == TRUE) return NULL;
    n = pl->first;
    data = (void *)n->data;
    pl->first = n->next;
    if (!pl->first) pl->last = NULL;
    pl->size--;
    free(n);
    return data;
}

void *list_popBack(List *pl) {
    Node *cur; void *data;
    if (!pl || list_isEmpty(pl) == TRUE) return NULL;
    if (pl->first == pl->last) {
        data = (void *)pl->last->data;
        free(pl->last);
        pl->first = NULL;
        pl->last = NULL;
        pl->size = 0;
        return data;
    }
    cur = pl->first;
    while (cur->next != pl->last) cur = cur->next;
    data = (void *)pl->last->data;
    free(pl->last);
    pl->last = cur;
    pl->last->next = NULL;
    pl->size--;
    return data;
}

void *list_getFront(List *pl) {
    if (!pl || list_isEmpty(pl) == TRUE) return NULL;
    return (void *)pl->first->data;
}

void *list_getBack(List *pl) {
    if (!pl || list_isEmpty(pl) == TRUE) return NULL;
    return (void *)pl->last->data;
}

void list_free(List *pl) {
    if (!pl) return;
    while (list_isEmpty(pl) == FALSE) list_popFront(pl);
    free(pl);
}

int list_size(const List *pl) {
    if (!pl) return ERROR_I;
    return pl->size;
}

int list_print(FILE *fp, const List *pl, p_list_ele_print f) {
    Node *cur; int chars = 0, w;
    if (!fp || !pl || !f) return ERROR_I;
    cur = pl->first;
    while (cur) {
        w = f(fp, cur->data);
        if (w < 0) return w;
        chars += w;
        cur = cur->next;
        if (cur) { w = fprintf(fp, "\n"); if (w < 0) return ERROR_I; chars += w; }
    }
    return chars;
}
