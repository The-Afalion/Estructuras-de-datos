/**
 * @file queue.c
 * @brief Circular queue implementation using pointer-based front/rear.
 */

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

#ifndef MAX_QUEUE
#define MAX_QUEUE 4096
#endif

struct _Queue {
    void *data[MAX_QUEUE];
    void **front;
    void **rear;
};

/**
 * @brief Checks whether the queue is full (private).
 *
 * The circular buffer sacrifices one slot to distinguish full from empty,
 * so capacity is MAX_QUEUE - 1.
 *
 * @param q Pointer to the queue.
 *
 * @return TRUE if full, FALSE otherwise.
 */
static Bool queue_isFull(const Queue *q) {
    if (!q) {
        return TRUE;
    }

    if (queue_isEmpty(q) == TRUE) {
        return FALSE;
    }

    return (queue_size(q) >= (size_t)(MAX_QUEUE - 1)) ? TRUE : FALSE;
}

Queue *queue_new() {
    Queue *q = NULL;
    int i;

    q = (Queue *)malloc(sizeof(Queue));
    if (!q) {
        return NULL;
    }

    q->front = NULL;
    q->rear = NULL;

    for (i = 0; i < MAX_QUEUE; i++) {
        q->data[i] = NULL;
    }

    return q;
}

void queue_free(Queue *q) {
    if (q) {
        free(q);
    }
}

Bool queue_isEmpty(const Queue *q) {
    if (!q || !q->front) {
        return TRUE;
    }

    return FALSE;
}

Status queue_push(Queue *q, void *ele) {
    if (!q || !ele || queue_isFull(q) == TRUE) {
        return ERROR;
    }

    if (queue_isEmpty(q) == TRUE) {
        q->front = (void **)q->data;
        q->rear = (void **)q->data;
    } else {
        q->rear++;
        if (q->rear >= (void **)(q->data + MAX_QUEUE)) {
            q->rear = (void **)q->data;
        }
    }

    *(q->rear) = ele;
    return OK;
}

void *queue_pop(Queue *q) {
    void *ele = NULL;

    if (!q || queue_isEmpty(q) == TRUE) {
        return NULL;
    }

    ele = *(q->front);

    if (q->front == q->rear) {
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front++;
        if (q->front >= (void **)(q->data + MAX_QUEUE)) {
            q->front = (void **)q->data;
        }
    }

    return ele;
}

void *queue_getFront(const Queue *q) {
    if (!q || queue_isEmpty(q) == TRUE) {
        return NULL;
    }

    return *(q->front);
}

void *queue_getBack(const Queue *q) {
    if (!q || queue_isEmpty(q) == TRUE) {
        return NULL;
    }

    return *(q->rear);
}

size_t queue_size(const Queue *q) {
    if (!q || queue_isEmpty(q) == TRUE) {
        return 0;
    }

    if (q->rear >= q->front) {
        return (size_t)(q->rear - q->front + 1);
    }

    return (size_t)(MAX_QUEUE - (q->front - q->rear) + 1);
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
    void **curr = NULL;
    int count = 0;
    int res;

    if (!fp || !q || !f) {
        return -1;
    }

    if (queue_isEmpty(q) == TRUE) {
        return 0;
    }

    curr = q->front;
    while (1) {
        res = f(fp, *curr);
        if (res < 0) {
            return -1;
        }
        count += res;
        count += fprintf(fp, "\n");

        if (curr == q->rear) {
            break;
        }

        curr++;
        if (curr >= (void **)(q->data + MAX_QUEUE)) {
            curr = (void **)q->data;
        }
    }

    return count;
}
