#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../stack.h"  // your header file

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

/******************* Tests *******************/

static stack* create_test_stack(ssize_t init_size) {
    stack *s = create_stack(init_size, copy_int, free_int, compare_int);
    assert(s != NULL);
    return s;
}

void test_create_stack(void) {
    stack *s1 = create_test_stack(10);
    assert(s1 != NULL);
    assert(stack_peek(s1) == NULL);
    assert(stack_pop(s1) == NULL); 
    free_stack(s1);

    stack *s2 = create_test_stack(0);
    assert(s2 != NULL);
    free_stack(s2);

    stack *s3 = create_test_stack(-5);
    assert(s3 != NULL);
    free_stack(s3);

    stack *s4 = create_test_stack(1000000);
    assert(s4 != NULL);
    free_stack(s4);

    printf("create stack test: PASS\n");
}

void test_stack_push(void) {
    stack *s = create_test_stack(10);

    int v1 = 20, v2 = 30, v3 = 40, v4 = 50;
    assert(stack_push(s, &v1) == 0);
    assert(*(int*)stack_peek(s) == 20);

    assert(stack_push(s, &v2) == 0);
    assert(stack_push(s, &v3) == 0);
    assert(*(int*)stack_peek(s) == 40);

    assert(stack_push(s, NULL) == -1);

    int *popped = stack_pop(s);
    assert(*(int*)popped == 40);
    assert(stack_push(s, &v4) == 0);

    for (int i = 0; i < 1000; i++) {
        assert(stack_push(s, &i) == 0);
    }

    assert(stack_push(NULL, &v1) == -1);

    free(popped);
    free_stack(s);
    printf("push test: PASS\n");
}

void test_stack_pop(void) {
    stack *s = create_test_stack(5);

    assert(stack_pop(s) == NULL);

    int v1 = 20;
    stack_push(s, &v1);
    int *popped = stack_pop(s);
    assert(*popped == 20);
    free(popped);

    int v2 = 30, v3 = 40, v4 = 50;
    stack_push(s, &v2);
    stack_push(s, &v3);
    stack_push(s, &v4);

    popped = stack_pop(s);
    assert(*popped == 50);
    free(popped);

    popped = stack_pop(s);
    assert(*popped == 40);
    free(popped);

    popped = stack_pop(s);
    assert(*popped == 30);
    free(popped);

    assert(stack_pop(s) == NULL);

    stack_push(s, NULL);
    assert(stack_pop(s) == NULL);

    assert(stack_pop(NULL) == NULL);

    free_stack(s);
    printf("pop test: PASS\n");
}

void test_stack_peek(void) {
    stack *s = create_test_stack(5);

    assert(stack_peek(s) == NULL);

    int v1 = 20, v2 = 40;
    stack_push(s, &v1);
    assert(*(int*)stack_peek(s) == 20);

    stack_push(s, &v2);
    assert(*(int*)stack_peek(s) == 40);

    int *popped = stack_pop(s);
    assert(*popped == 40);
    assert(*(int*)stack_peek(s) == 20);
    free(popped);

    void *p1 = stack_peek(s);
    void *p2 = stack_peek(s);
    assert(p1 == p2);

    assert(stack_peek(NULL) == NULL);

    int v3 = 40;
    stack_push(s, &v3);
    stack_push(s, NULL);
    assert(*(int*)stack_peek(s) == 40);

    free_stack(s);
    printf("peek test: PASS\n");
}

void test_free_stack(void) {
    stack *s1 = create_test_stack(10);
    free_stack(s1);

    stack *s2 = create_test_stack(10);
    int v1 = 1, v2 = 2;
    stack_push(s2, &v1);
    stack_push(s2, &v2);
    free_stack(s2);

    assert(free_stack(NULL) == -1);

    printf("free stack test: PASS\n");
}

int main(void) {
    printf("Running stack tests...\n");

    test_create_stack();
    test_stack_push();
    test_stack_pop();
    test_stack_peek();
    test_free_stack();

    printf("✅ All tests passed successfully!\n");
    return 0;
}

