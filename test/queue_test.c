#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../queue.h"

// ----------------- Helpers -----------------

static int* make_element_int(int v) {
    int* p = malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    return p;
}

static void free_int(void* p) {
    free(p);
}

// ----------------- Normal usage tests -----------------

static void test_enqueue_dequeue_front() {
    queue* qu = create_queue();
    assert(qu != NULL);

    // Enqueue elements
    assert(enqueue(qu, make_element_int(10)) == 0);
    assert(enqueue(qu, make_element_int(20)) == 0);
    assert(enqueue(qu, make_element_int(30)) == 0);

    // Front should return first element (10) without removing
    int* front = (int*)queue_front(qu);
    assert(front && *front == 10);

    // Dequeue should remove first element (10)
    int* deq = (int*)dequeue(qu);
    assert(deq && *deq == 10);
    free(deq);

    // Next front → 20
    front = (int*)queue_front(qu);
    assert(front && *front == 20);

    deq = (int*)dequeue(qu);
    assert(deq && *deq == 20);
    free(deq);

    deq = (int*)dequeue(qu);
    assert(deq && *deq == 30);
    free(deq);

    // Queue should now be empty
    assert(dequeue(qu) == NULL);
    assert(queue_front(qu) == NULL);

    free_queue(qu, free_int);
}

// ----------------- Edge cases -----------------

static void test_null_and_empty_queue() {
    // Operations on NULL queue
    int *num = make_element_int(1);
    assert(enqueue(NULL, num) == -1);
    assert(dequeue(NULL) == NULL);
    assert(queue_front(NULL) == NULL);
    assert(free_queue(NULL, free_int) == -1);

    free(num);
    // Free empty queue
    queue* qu = create_queue();
    assert(qu != NULL);
    assert(free_queue(qu, free_int) == 0);
}

static void test_free_element_null() {
    queue* qu = create_queue();
    assert(qu != NULL);

    // Enqueue an element
    int* a = make_element_int(100);
    assert(enqueue(qu, a) == 0);

    // Free queue with NULL → should not free element
    assert(free_queue(qu, NULL) == 0);
    free(a); // manual free because free_element=NULL
}

// ----------------- Stress test -----------------

static void test_stress_operations() {
    queue* qu = create_queue();
    const int N = 50;

    // Enqueue 0..49
    for (int i=0; i<N; ++i) assert(enqueue(qu, make_element_int(i)) == 0);

    // Front should be 0
    int* front = (int*)queue_front(qu);
    assert(front && *front == 0);

    // Dequeue all and check FIFO
    for (int i=0; i<N; ++i) {
        int* val = (int*)dequeue(qu);
        assert(val && *val == i);
        free(val);
    }

    // Queue empty now
    assert(dequeue(qu) == NULL);
    free_queue(qu, free_int);
}

int main(void) {
    // Normal
    test_enqueue_dequeue_front();

    // Edge
    test_null_and_empty_queue();
    test_free_element_null();

    // Stress
    test_stress_operations();

    printf("✅ All queue tests passed!\n");
    return 0;
}

