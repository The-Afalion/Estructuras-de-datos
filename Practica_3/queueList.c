/**
 * @file queueList.c
 * @brief Queue implementation backed by the List TAD.
 *
 * This module encapsulates a List inside the Queue structure and delegates
 * all FIFO operations to list_pushBack (enqueue) and list_popFront (dequeue).
 */

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "list.h"

struct _Queue {
    List *pl;
};

Queue *queue_new() {
    Queue *q = NULL;

    q = (Queue *)malloc(sizeof(Queue));
    if (!q) {
        return NULL;
    }

    q->pl = list_new();
    if (!q->pl) {
        free(q);
        return NULL;
    }

    return q;
}

void queue_free(Queue *q) {
    if (!q) {
        return;
    }

    list_free(q->pl);
    free(q);
}

Bool queue_isEmpty(const Queue *q) {
    if (!q) {
        return TRUE;
    }

    return list_isEmpty(q->pl);
}

Status queue_push(Queue *q, void *ele) {
    if (!q || !ele) {
        return ERROR;
    }

    return list_pushBack(q->pl, ele);
}

void *queue_pop(Queue *q) {
    if (!q) {
        return NULL;
    }

    return list_popFront(q->pl);
}

void *queue_getFront(const Queue *q) {
    if (!q) {
        return NULL;
    }

    return list_getFront(q->pl);
}

void *queue_getBack(const Queue *q) {
    if (!q) {
        return NULL;
    }

    return list_getBack(q->pl);
}

size_t queue_size(const Queue *q) {
    int s;

    if (!q) {
        return 0;
    }

    s = list_size(q->pl);
    return (s < 0) ? 0 : (size_t)s;
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
    int chars;

    if (!fp || !q || !f) {
        return -1;
    }

    chars = list_print(fp, q->pl, f);
    if (chars < 0) {
        return chars;
    }

    /* Add trailing newline to match array-based queue_print format */
    if (list_isEmpty(q->pl) == FALSE) {
        chars += fprintf(fp, "\n");
    }

    return chars;
}
