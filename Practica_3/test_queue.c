/* !!!!!!!!! MODIFICAR MAX_QUEUE a 8 !!!!!!! */
/* gcc queue.c test_queue.c -o test_queue */
/* valgrind --leak-check=full ./test_queue */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

#define MAX_TEST 8

/* ====== HELPERS ====== */
int int_print(FILE *fp, const void *a) {
    return fprintf(fp, "%d ", *(int*)a);
}

int *new_int(int v) {
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    return p;
}

void emptyQueue(Queue *q) {
	while(!queue_isEmpty(q) ){
		free(queue_pop(q));
	}
}

/* ====== TESTS ====== */
/* Creación e inicialización */
void test_queue_new() {
    Queue *q = queue_new();
    assert(q != NULL);
    assert(queue_isEmpty(q) == TRUE);
    assert(queue_size(q) == 0);
    assert(queue_getFront(q) == queue_getBack(q));
    queue_free(q);
}

/* Push básico */
void test_queue_push_basic() {
    Queue *q = queue_new();
    int *a = new_int(1);

    assert(queue_push(q, a) == OK);
    assert(queue_isEmpty(q) == FALSE);
    assert(queue_size(q) == 1);
    assert(*(int*)queue_getFront(q) == 1);
    assert(*(int*)queue_getBack(q) == 1);
	
	emptyQueue(q);
    queue_free(q);
}

/* FIFO correcto */
void test_queue_fifo() {
    Queue *q = queue_new();

    for (int i = 0; i < 5; i++) {
        assert(queue_push(q, new_int(i)) == OK);
    }

    for (int i = 0; i < 5; i++) {
        int *x = queue_pop(q);
        assert(x != NULL);
        assert(*x == i);
        free(x);
    }

    assert(queue_isEmpty(q) == TRUE);
    queue_free(q);
}

/* Pop en vacío */
void test_queue_pop_empty() {
    Queue *q = queue_new();

    assert(queue_pop(q) == NULL);
    assert(queue_getFront(q) == queue_getBack(q));

    queue_free(q);
}

/* Llenado hasta capacidad (MAX_QUEUE -1) */
void test_queue_full() {
    Queue *q = queue_new();
	int *overflow;

    for (int i = 0; i < MAX_TEST - 1; i++) {
        assert(queue_push(q, new_int(i)) == OK);
    }

    assert(queue_size(q) == MAX_TEST - 1);

    /* Debe fallar */
	overflow =  new_int(99);
    assert(queue_push(q, overflow) == ERROR);
	
	free(overflow);
	emptyQueue(q);
    queue_free(q);
}

/* Comportamiento circular */
void test_queue_circular() {
    Queue *q = queue_new();

    for (int i = 0; i < 5; i++) {
        queue_push(q, new_int(i));
    }

    for (int i = 0; i < 3; i++) {
        int *x = queue_pop(q);
        free(x);
    }

    for (int i = 5; i < 8; i++) {
        assert(queue_push(q, new_int(i)) == OK);
    }

    for (int i = 3; i < 8; i++) {
        int *x = queue_pop(q);
        assert(*x == i);
        free(x);
    }

    assert(queue_isEmpty(q) == TRUE);
    queue_free(q);
}

/* Front y Back correctos */
void test_front_back() {
    Queue *q = queue_new();

    queue_push(q, new_int(10));
    queue_push(q, new_int(20));
    queue_push(q, new_int(30));

    assert(*(int*)queue_getFront(q) == 10);
    assert(*(int*)queue_getBack(q) == 30);

    int *x = queue_pop(q);
    free(x);

    assert(*(int*)queue_getFront(q) == 20);

	emptyQueue(q);
    queue_free(q);
}

/* Tamaño correcto en operaciones mixtas */
void test_size_consistency() {
    Queue *q = queue_new();

    assert(queue_size(q) == 0);

    queue_push(q, new_int(1));
    queue_push(q, new_int(2));
    assert(queue_size(q) == 2);

    free(queue_pop(q));
    assert(queue_size(q) == 1);

    free(queue_pop(q));
    assert(queue_size(q) == 0);

	emptyQueue(q);
    queue_free(q);
}

/* Robustez con NULL */
void test_null_cases() {
    assert(queue_isEmpty(NULL) == TRUE);
    assert(queue_size(NULL) == 0);
    assert(queue_push(NULL, NULL) == ERROR);
    assert(queue_pop(NULL) == NULL);
    assert(queue_getFront(NULL) == NULL);
    assert(queue_getBack(NULL) == NULL);
}

/* Print */
void test_queue_print() {
    Queue *q = queue_new();

    queue_push(q, new_int(1));
    queue_push(q, new_int(2));
    queue_push(q, new_int(3));

    int chars = queue_print(stdout, q, int_print);
    assert(chars > 0);
	
	/* No se debe vaciar cola */
	assert(queue_isEmpty(q) == FALSE);

    printf("\n");
	emptyQueue(q);
    queue_free(q);
}

/* ====== MAIN ====== */
int main() {
    printf("Running tests...\n");

    test_queue_new();
    test_queue_push_basic();
    test_queue_fifo();
    test_queue_pop_empty();
    test_queue_full();
    test_queue_circular();
    test_front_back();
    test_size_consistency();
    test_null_cases();
    test_queue_print();

    printf("All tests passed!\n");
    return 0;
}
