#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

#define MAX_QUEUE 4096

struct _Queue {
    void *data[MAX_QUEUE];
    void **front;
    void **rear;
};

/* para saber si la cola esta llena (funcion privada)*/
Bool _queue_isFull(const Queue *q) {
    if (!q) return TRUE;
    if (q->front == NULL) return FALSE;
    
    void **next_rear = q->rear + 1;
    if (next_rear == q->data + MAX_QUEUE) {
        next_rear = (void **)q->data;
    }
    return (next_rear == q->front) ? TRUE : FALSE;
}

Queue *queue_new() {
    Queue *q = (Queue *) malloc(sizeof(struct _Queue));
    if (!q) return NULL;
    
    q->front = NULL;
    q->rear = NULL;

    for (int i = 0; i < MAX_QUEUE; i++) {
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
    if (!q || q->front == NULL) return TRUE;
    return FALSE;
}

Status queue_push(Queue *q, void *ele) {
    if (!q || !ele || _queue_isFull(q)) return ERROR;

    if (queue_isEmpty(q)) {
        q->front = (void **)q->data;
        q->rear = (void **)q->data;
    } else {
        q->rear++;
        if (q->rear == q->data + MAX_QUEUE) {
            q->rear = (void **)q->data;
        }
    }
    *(q->rear) = ele;
    return OK;
}

void *queue_pop(Queue *q) {
    if (!q || queue_isEmpty(q)) return NULL;
    void *ele = *(q->front);

    if (q->front == q->rear) { 
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front++;
        if (q->front == q->data + MAX_QUEUE) {
            q->front = (void **)q->data;
        }
    }

    return ele;
}

void *queue_getFront(const Queue *q) {
    if (!q || queue_isEmpty(q)) return NULL;
    return *(q->front);
}

void *queue_getBack(const Queue *q) {
    if (!q || queue_isEmpty(q)) return NULL;
    return *(q->rear);
}

size_t queue_size(const Queue *q) {
    if (!q || queue_isEmpty(q)) return 0;
    
    // Aritmética de punteros para calcular el tamaño
    if (q->rear >= q->front) {
        return (size_t)(q->rear - q->front + 1);
    } else {
        return (size_t)(MAX_QUEUE - (q->front - q->rear) + 1);
    }
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
    if (!fp || !q || !f) return -1;

    int count = 0;
    int res;
    
    count += fprintf(fp, "Queue (size %zu): ", queue_size(q));
    
    if (!queue_isEmpty(q)) {
        void **curr = q->front;
        while (curr != q->rear) {
            res = f(fp, *curr);
            if (res < 0) return -1;
            count += res;
            count += fprintf(fp, ", ");
            
            curr++;
            if (curr == q->data + MAX_QUEUE) {
                curr = (void **)q->data;
            }
        }
        // Impresion del utltimo elemento
        res = f(fp, *curr);
        if (res < 0) return -1;
        count += res;
    }
    
    count += fprintf(fp, "\n");
    return count;
}
