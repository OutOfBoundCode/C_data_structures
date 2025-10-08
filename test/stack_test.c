#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../stack.h"  // your header file

/******************* Helper functions *******************/

static stack* create_test_stack(ssize_t init_size) {
    stack *s = create_stack(init_size);
    assert(s != NULL);
    return s;
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

void test_create_stack(void) {
    // peek / pop / free empty stack
    stack *s1 = create_stack(10);
    assert(s1 != NULL);
    assert(stack_peek(s1) == NULL);
    assert(stack_pop(s1) == NULL); 
    free_stack(s1, NULL);

    // init_size = 0
    stack *s2 = create_stack(0); // should default to 10
    assert(s2 != NULL);
    free_stack(s2, NULL);

    // init_size < 0
    stack *s3 = create_stack(-5); // should default to 10
    assert(s3 != NULL);
    free_stack(s3, NULL);

    // very large init_size
    stack *s4 = create_stack(1000000);
    assert(s4 != NULL);
    free_stack(s4, NULL);

    printf("create list test: PASS\n");
}

void test_stack_push(void) {
    stack *s = create_test_stack(10);

    int *number = make_int(1);

    // push single element
    assert(stack_push(s, make_int(20)) == 0);
    assert(*(int*)stack_peek(s) == 20);

    // push multiple elements
    assert(stack_push(s, make_int(30)) == 0);
    assert(stack_push(s, make_int(40)) == 0);
    assert(*(int*)stack_peek(s) == 40);

    // push NULL
    assert(stack_push(s, NULL) == -1); // should failure and return -1

    // push after pop
    int *popped = stack_pop(s); // note: popping transefer memory ownership to the caller 
    assert(*popped == 40);
    assert(stack_push(s, make_int(50)) == 0);

    // push large number of elements
    for (int i = 0; i < 1000; i++) {
        assert(stack_push(s, make_int(i)) == 0);
    }

    // Error: push on NULL stack
    assert(stack_push(NULL, number) == -1);

    free_int(popped);
    free_int(number);
    free_stack(s, free_int);
    printf("push test: PASS\n");
}

void test_stack_pop(void) {
    stack *s = create_test_stack(5);

    int* popped = NULL;
    // pop from empty
    assert(stack_pop(s) == NULL);

    // pop single element
    stack_push(s, make_int(20));
    popped = stack_pop(s);
    assert(*popped == 20);
    free_int(popped);

    // LIFO order
    stack_push(s, make_int(30));
    stack_push(s, make_int(40));
    stack_push(s, make_int(50));

    popped = stack_pop(s);
    assert(*popped == 50);
    free_int(popped);

    popped = stack_pop(s);
    assert(*popped == 40);
    free_int(popped);

    popped = stack_pop(s);
    assert(*popped == 30);
    free_int(popped);
    
    assert(stack_pop(s) == NULL); // empty list

    // pop NULL element
    stack_push(s, NULL);
    assert(stack_pop(s) == NULL);

    // Error: pop NULL stack
    assert(stack_pop(NULL) == NULL); 

    free_stack(s, free_int);
    printf("pop test: PASS\n");
}

void test_stack_peek(void) {
    stack *s = create_test_stack(5);

    // peek empty
    assert(stack_peek(s) == NULL);

    // peek single element
    stack_push(s, make_int(20));
    assert(*(int*)stack_peek(s) == 20);

    // peek after multiple pushes
    stack_push(s, make_int(40));
    assert(*(int*)stack_peek(s) == 40);

    // peek after pop
    int* popped = stack_pop(s);
    assert(*(int*)popped == 40);
    assert(*(int*) stack_peek(s) == 20);

    // peek multiple times
    void *p1 = stack_peek(s);
    void *p2 = stack_peek(s);
    assert(p1 == p2);

    // peek NULL stack
    assert(stack_peek(NULL) == NULL);

    // peek after pushing NULL
    stack_push(s, make_int(40)); 
    stack_push(s, NULL); // should fail
    assert(*(int*)stack_peek(s) == 40); // should peek the last non-NULL pushed element

    free_int(popped);
    free_stack(s, free_int);
    printf("peek test: PASS\n");
}

void test_free_stack(void) {
    // free empty stack
    stack *s1 = create_stack(10);
    assert(free_stack(s1, NULL) == 0);

    // free non-empty stack
    stack *s2 = create_stack(10);
    stack_push(s2, make_int(1));
    stack_push(s2, make_int(2));
    assert(free_stack(s2, free_int) == 0);

    // free stack that doesn't own the memory
    stack *s3 = create_stack(10);
    stack_push(s3, "text1");
    stack_push(s3, "text2");
    assert(free_stack(s3, NULL) == 0); // pass free_element NULL to avoid freeing memory not owned by the stack

    // free NULL stack
    assert(free_stack(NULL, NULL) == -1);
    printf("free list test: PASS\n");
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
