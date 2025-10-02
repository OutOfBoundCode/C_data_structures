#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../stack.h"

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

static void test_push_pop_peek() {
    stack* st = create_stack(5);
    assert(st != NULL);

    // Push elements
    assert(stack_push(st, make_element_int(10)) == 0);
    assert(stack_push(st, make_element_int(20)) == 0);
    assert(stack_push(st, make_element_int(30)) == 0);

    // Peek should return last pushed (30) without removing
    int* top = (int*)stack_peek(st);
    assert(top && *top == 30);

    // Pop should remove last pushed
    int* popped = (int*)stack_pop(st);
    assert(popped && *popped == 30);
    free(popped);

    // Next peek → 20
    top = (int*)stack_peek(st);
    assert(top && *top == 20);

    popped = (int*)stack_pop(st);
    assert(popped && *popped == 20);
    free(popped);

    popped = (int*)stack_pop(st);
    assert(popped && *popped == 10);
    free(popped);

    // Stack should now be empty
    assert(stack_pop(st) == NULL);
    assert(stack_peek(st) == NULL);

    free_stack(st, free_int);
}

// ----------------- Edge cases -----------------

static void test_null_and_empty_stack() {
    // Operations on NULL stack
    int *num = make_element_int(1);
    assert(stack_push(NULL, num) == -1);
    assert(stack_pop(NULL) == NULL);
    assert(stack_peek(NULL) == NULL);
    assert(free_stack(NULL, free_int) == -1);
    free(num);

    // Free empty stack
    stack* st = create_stack(5);
    assert(st != NULL);
    assert(free_stack(st, free_int) == 0);
}

static void test_free_element_null() {
    stack* st = create_stack(5);
    assert(st != NULL);

    // Push an element
    int* a = make_element_int(100);
    assert(stack_push(st, a) == 0);

    // Free with NULL → should not free element
    assert(free_stack(st, NULL) == 0);
    free(a); // manual free because free_element=NULL
}

// ----------------- Stress test -----------------

static void test_stress_operations() {
    stack* st = create_stack(50);
    const int N = 50;

    // Push 0..49
    for (int i=0; i<N; ++i) assert(stack_push(st, make_element_int(i)) == 0);

    // Peek should be N-1
    int* top = (int*)stack_peek(st);
    assert(top && *top == N-1);

    // Pop all and check LIFO
    for (int i=N-1; i>=0; --i) {
        int* val = (int*)stack_pop(st);
        assert(val && *val == i);
        free(val);
    }

    // Stack empty now
    assert(stack_pop(st) == NULL);
    free_stack(st, free_int);
}

int main(void) {
    test_push_pop_peek();
    test_null_and_empty_stack();
    test_free_element_null();
    test_stress_operations();

    printf("✅ All stack tests passed!\n");
    return 0;
}

