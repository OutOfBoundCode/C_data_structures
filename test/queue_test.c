#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../queue.h"  // your queue header

/******************* Helper functions *******************/

static queue* create_test_queue(void) {
    queue *q = create_queue();
    assert(q != NULL);
    return q;
}

static int* make_int(int value) {
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = value;
    return p;
}

static void free_int(void *ptr) {
    free(ptr);
}

/******************* Tests *******************/

void test_create_queue(void) {
    queue *q = create_queue();
    assert(q != NULL);
    assert(free_queue(q, NULL) == 0);
    printf("create queue test: PASS\n");
}

void test_enqueue(void) {
    queue *q = create_test_queue();

    // Enqueue single element
    assert(enqueue(q, make_int(10)) == 0);
    assert(*(int*)queue_front(q) == 10);

    // Enqueue multiple elements
    assert(enqueue(q, make_int(20)) == 0);
    assert(enqueue(q, make_int(30)) == 0);
    assert(*(int*)queue_front(q) == 10); // front should still be first element

    // Enqueue NULL
    assert(enqueue(q, NULL) == -1); // should fail

    // enqueue on NULL queue
    int *number = make_int(40);
    assert(enqueue(NULL, number) == -1); // should fail
    free_int(number); // queue didn't take ownership

    free_queue(q, free_int);
    printf("enqueue test: PASS\n");
}

void test_dequeue(void) {
    queue *q = create_test_queue();

    int *popped = NULL;

    // Dequeue empty queue
    assert(dequeue(q) == NULL);

    // Dequeue single element
    enqueue(q, make_int(10));
    popped = dequeue(q);
    assert(*popped == 10);
    free_int(popped);

    // Dequeue multiple elements (FIFO order)
    enqueue(q, make_int(20));
    enqueue(q, make_int(30));
    enqueue(q, make_int(40));

    popped = dequeue(q);
    assert(*popped == 20);
    free_int(popped);

    popped = dequeue(q);
    assert(*popped == 30);
    free_int(popped);

    popped = dequeue(q);
    assert(*popped == 40);
    free_int(popped);

    // Queue is now empty
    assert(dequeue(q) == NULL);

    // Dequeue on NULL queue
    assert(dequeue(NULL) == NULL);

    free_queue(q, free_int);
    printf("dequeue test: PASS\n");
}

void test_queue_front(void) {
    queue *q = create_test_queue();

    // Front on empty queue
    assert(queue_front(q) == NULL);

    // Front after enqueue
    enqueue(q, make_int(10));
    assert(*(int*)queue_front(q) == 10);

    enqueue(q, make_int(20));
    assert(*(int*)queue_front(q) == 10); // front should still be first element

    // Front after dequeue
    int *popped = dequeue(q);
    free_int(popped);
    assert(*(int*)queue_front(q) == 20);

    // Multiple front calls
    void *f1 = queue_front(q);
    void *f2 = queue_front(q);
    assert(f1 == f2);

    // Front on NULL queue
    assert(queue_front(NULL) == NULL);

    free_queue(q, free_int);
    printf("queue front test: PASS\n");
}

void test_free_queue(void) {
    // Free empty queue
    queue *q1 = create_queue();
    assert(free_queue(q1, NULL) == 0);

    // Free non-empty queue
    queue *q2 = create_queue();
    enqueue(q2, make_int(1));
    enqueue(q2, make_int(2));
    assert(free_queue(q2, free_int) == 0);

    // Queue with external memory (not owned)
    queue *q3 = create_queue();
    enqueue(q3, "text1");
    enqueue(q3, "text2");
    assert(free_queue(q3, NULL) == 0);

    // Free NULL queue
    assert(free_queue(NULL, NULL) == -1);

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

