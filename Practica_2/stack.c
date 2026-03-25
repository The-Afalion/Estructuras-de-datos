#include "stack.h"
#include <stdlib.h>

#define INIT_CAPACITY 2
#define FCT_CAPACITY  2

struct _Stack {
    void **item;
    int top;
    int capacity;
};

Stack * stack_init() {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s) {
        return NULL;
    }
    
    s->capacity = INIT_CAPACITY;
    s->item = (void **)malloc(s->capacity * sizeof(void *));
    if (!s->item) {
        free(s);
        return NULL;
    }
    
    s->top = -1;
    return s;
}

void stack_free(Stack *s) {
    if (!s) {
        return;
    }
    if (s->item) {
        free(s->item);
    }
    free(s);
}

Status stack_push(Stack *s, const void *ele) {
    void **new_item = NULL;
    
    if (!s || !ele) {
        return ERROR;
    }
    
    if (s->top == s->capacity - 1) {
        s->capacity *= FCT_CAPACITY;
        new_item = (void **)realloc(s->item, s->capacity * sizeof(void *));
        if (!new_item) {
            return ERROR;
        }
        s->item = new_item;
    }
    
    s->top++;
    s->item[s->top] = (void *)ele;
    
    return OK;
}

void * stack_pop(Stack *s) {
    void *ele = NULL;
    
    if (!s || stack_isEmpty(s) == TRUE) {
        return NULL;
    }
    
    ele = s->item[s->top];
    s->top--;
    
    return ele;
}

void * stack_top(const Stack *s) {
    if (!s || stack_isEmpty(s) == TRUE) {
        return NULL;
    }
    
    return s->item[s->top];
}

Bool stack_isEmpty(const Stack *s) {
    if (!s) {
        return TRUE;
    }
    if (s->top == -1) {
        return TRUE;
    }
    
    return FALSE;
}

size_t stack_size(const Stack *s) {
    if (!s) {
        return 0;
    }
    
    return (size_t)(s->top + 1);
}

int stack_print(FILE* fp, const Stack *s, P_stack_ele_print f) {
    int i, chars = 0, res = 0;
    
    if (!fp || !s || !f) {
        return -1;
    }
    
    for (i = s->top; i >= 0; i--) {
        res = f(fp, s->item[i]);
        if (res < 0) {
            return -1;
        }
        chars += res;
        chars += fprintf(fp, "\n");
    }
    
    return chars;
}
