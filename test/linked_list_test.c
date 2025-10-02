#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h> 
#include "../linked_list.h"

// ----------------- Helper functions -----------------

static int *make_element_int(int v) {
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    return p;
}

static void free_int(void *p) {
    free(p);
}

static int compare_int(void *a, void *b) {
    if (a == NULL || b == NULL) return -1;
    int ia = *(int*)a;
    int ib = *(int*)b;
    return (ia == ib) ? 0 : -1;
}

static void print_int(void *p) {
    if (p == NULL) return;
    printf("[%d]", *(int*)p);
}

// ----------------- Tests -----------------

static void test_create_and_append() {
    linked_list *list = create_linked_list();
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->tail == NULL);

    assert(llappend(list, make_element_int(10)) == 0);
    assert(llappend(list, make_element_int(20)) == 0);
    assert(list->length == 2);

    int *a = (int *)llget(list, 0);
    int *b = (int *)llget(list, 1);
    assert(a != NULL && b != NULL);
    assert(*a == 10 && *b == 20);

    free_linked_list(list, free_int);
}

static void test_add_set_get() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(1)) == 0);
    assert(llappend(list, make_element_int(3)) == 0);

    // Insert 2 at index 1 → [1,2,3]
    assert(lladd(list, 1, make_element_int(2)) == 0);
    assert(list->length == 3);
    int *val = (int *)llget(list, 1);
    assert(val != NULL && *val == 2);

    assert(llset(list, 1, make_element_int(42), free_int) == 0);
    val = (int *)llget(list, 1);
    assert(val != NULL && *val == 42);

    free_linked_list(list, free_int);
}

static void test_index_delete() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(5)) == 0);
    assert(llappend(list, make_element_int(10)) == 0);
    assert(llappend(list, make_element_int(15)) == 0);

    int target = 10;
    ssize_t idx = llget_index(list, &target, compare_int);
    assert(idx == 1);

    // Delete index 1 → [5,15]
    assert(lldelete(list, 1, free_int) == 0);
    assert(list->length == 2);
    int *val = (int *)llget(list, 1);
    assert(val != NULL && *val == 15);

    free_linked_list(list, free_int);
}

static void test_pop_reverse() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(1)) == 0);
    assert(llappend(list, make_element_int(2)) == 0);
    assert(llappend(list, make_element_int(3)) == 0);

    // Reverse → [3,2,1]
    llreverse(list);
    int *a = (int *)llget(list, 0);
    int *b = (int *)llget(list, 1);
    int *c = (int *)llget(list, 2);
    assert(a != NULL && b != NULL && c != NULL);
    assert(*a == 3 && *b == 2 && *c == 1);

    // Pop last → removes 1
    assert(llpop(list, free_int) == 0);
    assert(list->length == 2);
    int *last = (int *)llget(list, list->length - 1);
    assert(last != NULL && *last == 2);

    free_linked_list(list, free_int);
}

// ----------------- Edge Case Tests -----------------

static void test_null_and_invalid_inputs() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    int *p = make_element_int(2);
    int *q = make_element_int(5);

    // NULL list arguments should return error codes or no-op according to API
    assert(llappend(NULL, p) == -1);
    assert(lladd(NULL, 0, p) == -1);
    assert(llset(NULL, 0, q, free_int) == -1);
    assert(llget_index(NULL, 0, compare_int) == -1);
    assert(llget_index(list, 0, NULL) == -1);
    assert(llget(NULL, 0) == NULL);
    assert(lldelete(NULL, 0, free_int) == -1);
    assert(llpop(NULL, free_int) == -1);
    llreverse(NULL);   // should be a safe no-op
    llprint(NULL, print_int); // should be a safe no-op

    free_linked_list(NULL, free_int); // should be safe

    // Invalid indices on a real list
    assert(llappend(list, q) == 0);
    // negative index should be treated as invalid; llget should return NULL
    assert(llget(list, -1) == NULL);
    // out-of-bounds read
    assert(llget(list, 1) == NULL);
    // out-of-bounds add
    assert(lladd(list, 2, q) == -1);
    // out-of-bounds delete
    assert(lldelete(list, 1, free_int) == -1);
    // out-of-bounds set
    assert(llset(list, 1, p, free_int) == -1);

    // Clean up
    free(p);
    free_linked_list(list, free_int);
}

static void test_empty_list_operations() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llpop(list, free_int) == -1);
    assert(lldelete(list, 0, free_int) == -1);
    // reverse & print on empty list should be safe
    llreverse(list);
    llprint(list, print_int);

    free_linked_list(list, free_int);
}

static void test_free_element_null() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(100)) == 0);

    // If user provides NULL free function to llset, old element should NOT be freed by list
    int *old_ptr = (int *)llget(list, 0);
    assert(old_ptr != NULL);

    assert(llset(list, 0, make_element_int(200), NULL) == 0);
    // The original 100 is still allocated (leaked by API because user passed NULL); free it manually to avoid leak in test
    free(old_ptr);

    free_linked_list(list, free_int);
}

static void test_llget_index_not_found() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(1)) == 0);
    assert(llappend(list, make_element_int(2)) == 0);

    int target = 42;
    ssize_t idx = llget_index(list, &target, compare_int);
    assert(idx == -1);

    free_linked_list(list, free_int);
}

static void test_head_tail_length_invariants() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(list->head == NULL && list->tail == NULL && list->length == 0);

    assert(llappend(list, make_element_int(1)) == 0);
    assert(list->head != NULL && list->tail != NULL && list->length == 1);

    assert(llappend(list, make_element_int(2)) == 0);
    assert(list->tail->value != NULL && list->length == 2);

    // Delete head, ensure length invariant
    assert(lldelete(list, 0, free_int) == 0);
    assert(list->length == 1);

    // Pop remaining element → list becomes empty
    assert(llpop(list, free_int) == 0);
    assert(list->head == NULL && list->tail == NULL && list->length == 0);

    free_linked_list(list, free_int);
}

// Optional: a stress test to exercise many operations (keeps runtime small)
static void test_stress_operations() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    const int N = 100;
    for (int i = 0; i < N; ++i) {
        assert(llappend(list, make_element_int(i)) == 0);
    }
    assert(list->length == N);

    // reverse and check first/last
    llreverse(list);
    int *first = (int *)llget(list, 0);
    int *last = (int *)llget(list, list->length - 1);
    assert(first != NULL && last != NULL);
    assert(*first == N - 1 && *last == 0);

    // delete a middle range
    for (int i = 0; i < 10; ++i) {
        assert(lldelete(list, 10, free_int) == 0);
    }

    // set a few values
    for (int i = 0; i < 5; ++i) {
        assert(llset(list, i, make_element_int(i * 1000), free_int) == 0);
    }

    free_linked_list(list, free_int);
}

int main(void) {
    // Normal operations
    test_create_and_append();
    test_add_set_get();
    test_index_delete();
    test_pop_reverse();

    // Edge cases
    test_null_and_invalid_inputs();
    test_empty_list_operations();
    test_free_element_null();
    test_llget_index_not_found();
    test_head_tail_length_invariants();

    // Stress
    test_stress_operations();

    printf("✅ All linked_list tests passed!\n");
    return 0;
}

