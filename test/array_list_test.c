#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include "../array_list.h"

// ----------------- Helpers functions -----------------

static int* make_element_int(int v) {
    int* p = malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    *p = v;
    return p;
}

static void free_int(void* p) {
    free(p);
}

static int compare_int(void* a, void* b) {
    if (!a || !b) return -1;
    return (*(int*)a == *(int*)b) ? 0 : -1;
}

static void print_int(void* p) {
    if (p) printf("[%d]", *(int*)p);
}

// ----------------- Normal usage tests -----------------

static void test_create_and_append() {
    array_list* list = create_array_list(10);
    assert(list != NULL);
    assert(list->length == 0);

    assert(alappend(list, make_element_int(10)) == 0);
    assert(alappend(list, make_element_int(20)) == 0);
    assert(list->length == 2);

    int* a = (int*)alget(list, 0);
    int* b = (int*)alget(list, 1);
    assert(a && b && *a == 10 && *b == 20);

    free_array_list(list, free_int);
}

static void test_add_set_get() {
    array_list* list = create_array_list(10);
    alappend(list, make_element_int(1));
    alappend(list, make_element_int(3));

    // Insert 2 at index 1 → [1,2,3]
    assert(aladd(list, 1, make_element_int(2)) == 0);
    assert(list->length == 3);
    int* val = (int*)alget(list, 1);
    assert(val && *val == 2);

    // Replace index 1 with 42
    assert(alset(list, 1, make_element_int(42), free_int) == 0);
    val = (int*)alget(list, 1);
    assert(val && *val == 42);

    free_array_list(list, free_int);
}

static void test_index_delete() {
    array_list* list = create_array_list(10);
    alappend(list, make_element_int(5));
    alappend(list, make_element_int(10));
    alappend(list, make_element_int(15));

    int target = 10;
    ssize_t idx = alget_index(list, &target, compare_int);
    assert(idx == 1);

    // Delete index 1 → [5,15]
    assert(aldelete(list, 1, free_int) == 0);
    assert(list->length == 2);
    int* val = (int*)alget(list, 1);
    assert(val && *val == 15);

    free_array_list(list, free_int);
}

static void test_pop_reverse() {
    array_list* list = create_array_list(10);
    alappend(list, make_element_int(1));
    alappend(list, make_element_int(2));
    alappend(list, make_element_int(3));

    // Reverse → [3,2,1]
    alreverse(list);
    int* a = (int*)alget(list, 0);
    int* b = (int*)alget(list, 1);
    int* c = (int*)alget(list, 2);
    assert(a && b && c && *a == 3 && *b == 2 && *c == 1);

    // Pop last → removes 1
    assert(alpop(list, free_int) == 0);
    assert(list->length == 2);
    int* last = (int*)alget(list, list->length - 1);
    assert(last && *last == 2);

    free_array_list(list, free_int);
}

// ----------------- Edge cases -----------------

static void test_null_and_invalid_inputs() {
    array_list* list = create_array_list(5);
    int* p = make_element_int(2);
    int* q = make_element_int(5);

    assert(alappend(NULL, p) == -1);
    assert(aladd(NULL, 0, p) == -1);
    assert(alset(NULL, 0, q, free_int) == -1);
    assert(alget(NULL, 0) == NULL);
    assert(aldelete(NULL, 0, free_int) == -1);
    assert(alpop(NULL, free_int) == -1);
    alreverse(NULL);
    alprint(NULL, print_int);

    // Invalid indices
    alappend(list, q);
    assert(alget(list, -1) == NULL);
    assert(alget(list, list->length) == NULL);
    assert(aladd(list, list->length+1, q) == -1);
    assert(aldelete(list, list->length, free_int) == -1);
    assert(alset(list, list->length, p, free_int) == -1);

    free(p);
    free_array_list(list, free_int);
}

static void test_empty_list_operations() {
    array_list* list = create_array_list(5);

    assert(alpop(list, free_int) == -1);
    assert(aldelete(list, 0, free_int) == -1);
    alreverse(list);
    alprint(list, print_int);

    free_array_list(list, free_int);
}

static void test_free_element_null() {
    array_list* list = create_array_list(5);
    alappend(list, make_element_int(100));

    int* old_ptr = (int*)alget(list, 0);
    assert(alset(list, 0, make_element_int(200), NULL) == 0);
    free(old_ptr); // free manually because free_element=NULL

    free_array_list(list, free_int);
}

static void test_alget_index_not_found() {
    array_list* list = create_array_list(5);
    alappend(list, make_element_int(1));
    alappend(list, make_element_int(2));

    int target = 42;
    ssize_t idx = alget_index(list, &target, compare_int);
    assert(idx == -1);

    free_array_list(list, free_int);
}

static void test_length_maxsize_invariants() {
    array_list* list = create_array_list(5);
    assert(list->length == 0 && list->max_size == 5);

    alappend(list, make_element_int(1));
    assert(list->length == 1);

    alappend(list, make_element_int(2));
    assert(list->length == 2);

    aldelete(list, 0, free_int);
    assert(list->length == 1);

    alpop(list, free_int);
    assert(list->length == 0);

    free_array_list(list, free_int);
}

// Optional stress test
static void test_stress_operations() {
    array_list* list = create_array_list(50);
    const int N = 50;
    for (int i=0; i<N; ++i) alappend(list, make_element_int(i));

    alreverse(list);
    int* first = (int*)alget(list, 0);
    int* last = (int*)alget(list, list->length-1);
    assert(first && last && *first == N-1 && *last == 0);

    for (int i=0; i<10; ++i) aldelete(list, 10, free_int);
    for (int i=0; i<5; ++i) alset(list, i, make_element_int(i*1000), free_int);

    free_array_list(list, free_int);
}

int main(void) {
    // Normal
    test_create_and_append();
    test_add_set_get();
    test_index_delete();
    test_pop_reverse();

    // Edge
    test_null_and_invalid_inputs();
    test_empty_list_operations();
    test_free_element_null();
    test_alget_index_not_found();
    test_length_maxsize_invariants();

    // Stress
    test_stress_operations();

    printf("✅ All array_list tests passed!\n");
    return 0;
}

