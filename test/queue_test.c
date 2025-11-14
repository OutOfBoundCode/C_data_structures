#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../queue.h"

/******************* Helper functions *******************/

void free_int(void *ptr) {
    free(ptr);
}

void* copy_int(void *p) {
    if (!p) return NULL;
    int *c = malloc(sizeof(int));
    if (!c) return NULL;
    *c = *(int*)p;
    return c;
}

int compare_int(void *a, void *b) {
    if (!a || !b) return -1;
    return (*(int*)a == *(int*)b) ? 0 : -1;
}

static queue* create_test_queue(void) {
    queue *q = create_queue(copy_int, free_int, compare_int);
    assert(q != NULL);
    return q;
}

/******************* Tests *******************/

void test_create_queue(void) {
    queue *q = create_test_queue();
    assert(q != NULL);
    assert(free_queue(q) == 0);
    printf("create queue test: PASS\n");
}

void test_enqueue(void) {
    queue *q = create_test_queue();

    int v1 = 10, v2 = 20, v3 = 30, v4 = 40;

    assert(enqueue(q, &v1) == 0);
    assert(*(int*)queue_front(q) == 10);

    assert(enqueue(q, &v2) == 0);
    assert(enqueue(q, &v3) == 0);
    assert(*(int*)queue_front(q) == 10); // front remains first element

    assert(enqueue(q, NULL) == -1); // NULL element should fail

    assert(enqueue(NULL, &v4) == -1); // enqueue on NULL queue should fail

    free_queue(q);
    printf("enqueue test: PASS\n");
}

void test_dequeue(void) {
    queue *q = create_test_queue();
    int *popped = NULL;

    // Dequeue empty queue
    assert(dequeue(q) == NULL);

    int v1 = 10;
    enqueue(q, &v1);
    popped = dequeue(q);
    assert(*popped == 10);
    free(popped);

    int v2 = 20, v3 = 30, v4 = 40;
    enqueue(q, &v2);
    enqueue(q, &v3);
    enqueue(q, &v4);

    popped = dequeue(q);
    assert(*popped == 20);
    free(popped);

    popped = dequeue(q);
    assert(*popped == 30);
    free(popped);

    popped = dequeue(q);
    assert(*popped == 40);
    free(popped);

    assert(dequeue(q) == NULL);  // queue empty

    assert(dequeue(NULL) == NULL); // NULL queue

    free_queue(q);
    printf("dequeue test: PASS\n");
}

void test_queue_front(void) {
    queue *q = create_test_queue();

    assert(queue_front(q) == NULL); // empty queue

    int v1 = 10, v2 = 20;
    enqueue(q, &v1);
    assert(*(int*)queue_front(q) == 10);

    enqueue(q, &v2);
    assert(*(int*)queue_front(q) == 10); // front remains first

    int *popped = dequeue(q);
    free(popped);
    assert(*(int*)queue_front(q) == 20);

    void *f1 = queue_front(q);
    void *f2 = queue_front(q);
    assert(f1 == f2);

    assert(queue_front(NULL) == NULL);

    free_queue(q);
    printf("queue front test: PASS\n");
}

void test_free_queue(void) {
    // empty queue
    queue *q1 = create_test_queue();
    assert(free_queue(q1) == 0);

    // non-empty queue
    queue *q2 = create_test_queue();
    int v1 = 1, v2 = 2;
    enqueue(q2, &v1);
    enqueue(q2, &v2);
    assert(free_queue(q2) == 0);

    // NULL queue
    assert(free_queue(NULL) == -1);

    printf("free queue test: PASS\n");
}

int main(void) {
    printf("Running queue tests...\n");

    test_create_queue();
    test_enqueue();
    test_dequeue();
    test_queue_front();
    test_free_queue();

    printf("✅ All tests passed successfully!\n");
    return 0;
}

