#include "list.h"

typedef struct _Node {
    const void *data;
    struct _Node *next;
} Node;

struct _List {
    Node *front;
    Node *rear;
    int size;
};

/**
 * @brief Allocates and initializes one list node.
 *
 * @param e Element to store by reference.
 *
 * @return Pointer to the created node, or NULL on error.
 */
static Node *node_new(const void *e) {
    Node *node = NULL;

    if (!e) {
        return NULL;
    }

    node = (Node *)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }

    node->data = e;
    node->next = NULL;

    return node;
}

List *list_new() {
    List *pl = NULL;

    pl = (List *)malloc(sizeof(List));
    if (!pl) {
        return NULL;
    }

    pl->front = NULL;
    pl->rear = NULL;
    pl->size = 0;

    return pl;
}

Bool list_isEmpty(const List *pl) {
    if (!pl || pl->size == 0) {
        return TRUE;
    }

    return FALSE;
}

Status list_pushFront(List *pl, const void *e) {
    Node *node = NULL;

    if (!pl || !e) {
        return ERROR;
    }

    node = node_new(e);
    if (!node) {
        return ERROR;
    }

    node->next = pl->front;
    pl->front = node;
    if (!pl->rear) {
        pl->rear = node;
    }
    pl->size++;

    return OK;
}

Status list_pushBack(List *pl, const void *e) {
    Node *node = NULL;

    if (!pl || !e) {
        return ERROR;
    }

    node = node_new(e);
    if (!node) {
        return ERROR;
    }

    if (list_isEmpty(pl) == TRUE) {
        pl->front = node;
        pl->rear = node;
    } else {
        pl->rear->next = node;
        pl->rear = node;
    }
    pl->size++;

    return OK;
}

void *list_popFront(List *pl) {
    Node *node = NULL;
    void *data = NULL;

    if (!pl || list_isEmpty(pl) == TRUE) {
        return NULL;
    }

    node = pl->front;
    data = (void *)node->data;
    pl->front = node->next;
    if (!pl->front) {
        pl->rear = NULL;
    }
    pl->size--;

    free(node);
    return data;
}

void *list_popBack(List *pl) {
    Node *current = NULL;
    void *data = NULL;

    if (!pl || list_isEmpty(pl) == TRUE) {
        return NULL;
    }

    if (pl->front == pl->rear) {
        data = (void *)pl->rear->data;
        free(pl->rear);
        pl->front = NULL;
        pl->rear = NULL;
        pl->size = 0;
        return data;
    }

    current = pl->front;
    while (current->next != pl->rear) {
        current = current->next;
    }

    data = (void *)pl->rear->data;
    free(pl->rear);
    pl->rear = current;
    pl->rear->next = NULL;
    pl->size--;

    return data;
}

void *list_getFront(List *pl) {
    if (!pl || list_isEmpty(pl) == TRUE) {
        return NULL;
    }

    return (void *)pl->front->data;
}

void *list_getBack(List *pl) {
    if (!pl || list_isEmpty(pl) == TRUE) {
        return NULL;
    }

    return (void *)pl->rear->data;
}

void list_free(List *pl) {
    if (!pl) {
        return;
    }

    while (list_isEmpty(pl) == FALSE) {
        list_popFront(pl);
    }

    free(pl);
}

int list_size(const List *pl) {
    if (!pl) {
        return ERROR_I;
    }

    return pl->size;
}

int list_print(FILE *fp, const List *pl, p_list_ele_print f) {
    Node *current = NULL;
    int chars = 0;
    int written = 0;

    if (!fp || !pl || !f) {
        return ERROR_I;
    }

    current = pl->front;
    while (current) {
        written = f(fp, current->data);
        if (written < 0) {
            return written;
        }
        chars += written;
        current = current->next;
        if (current) {
            written = fprintf(fp, "\n");
            if (written < 0) {
                return ERROR_I;
            }
            chars += written;
        }
    }

    return chars;
}
