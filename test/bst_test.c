#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../bst.h"

// ----------------- Helper functions -----------------

void free_int(void *p) { free(p); }

void *copy_int(void *p) {
    if (!p) return NULL;
    int *c = malloc(sizeof(int));
    *c = *(int*)p;
    return c;
}

int compare_int(void *a, void *b) {
    if (!a || !b) return -1;
    int ia = *(int*)a;
    int ib = *(int*)b;
    return (ia > ib) - (ia < ib);
}

void print_int(void *p) { if (p) printf("%d", *(int*)p); }

// ----------------- Tests -----------------

void test_create_bst() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    assert(t != NULL);
    assert(t->root == NULL);
    free_bst(t);
    free_bst(NULL);
    printf("test_create_bst: PASS\n");
}

void test_bstadd() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    int vals[] = {10, 5, 15, 2, 7};
    for (int i = 0; i < 5; i++)
        assert(bstadd(t, &vals[i]) == 0);

    // Duplicate should fail 
    assert(bstadd(t, &vals[0]) == -1);

    // Null inputs
    assert(bstadd(NULL, &vals[0]) == -1);
    assert(bstadd(t, NULL) == -1);

    free_bst(t);
    printf("test_bstadd: PASS\n");
}

void test_bstsearch() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    int vals[] = {10, 5, 15};
    for (int i = 0; i < 3; i++) bstadd(t, &vals[i]);

    int exists = 5;
    int not_exists = 8;
    assert(bstsearch(t, &exists) == 0);
    assert(bstsearch(t, &not_exists) == -1);

    assert(bstsearch(NULL, &exists) == -1);
    assert(bstsearch(t, NULL) == -1);

    free_bst(t);
    printf("test_bstsearch: PASS\n");
}

void test_bstdelete() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    int vals[] = {10, 5, 15, 2, 7, 12, 20};
    for (int i = 0; i < 7; i++) bstadd(t, &vals[i]);

    // Delete leaf
    int leaf = 2;
    assert(bstdelete(t, &leaf) == 0);
    assert(bstsearch(t, &leaf) == -1);

    // Delete node with one child
    int one_child = 15;
    assert(bstdelete(t, &one_child) == 0);
    assert(bstsearch(t, &one_child) == -1);

    // Delete node with two children
    int two_children = 5;
    assert(bstdelete(t, &two_children) == 0);
    assert(bstsearch(t, &two_children) == -1);

    // Delete root
    int root = 10;
    assert(bstdelete(t, &root) == 0);
    assert(bstsearch(t, &root) == -1);

    // Delete non-existent
    int nonexist = 99;
    assert(bstdelete(t, &nonexist) == -1);

    // Delete null
    assert(bstdelete(t, NULL) == -1);

    // Delete from empty tree
    bst *empty = create_bst(copy_int, free_int, compare_int);
    int val = 5;
    assert(bstdelete(empty, &val) == -1);
    free_bst(empty);

    free_bst(t);
    printf("test_bstdelete: PASS\n");
}

void test_insert_ordered_list() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    
    int nums[] = {1,2,3,4,5,6,7};

    void* elements[7];

    for (int i = 0; i < 7; i++) elements[i] = nums + i;

    insert_ordered_list(t, elements, 0, 6);

    for (int i = 0; i < 7; i++)
        assert(bstsearch(t, elements[i]) == 0);

    // Null tree
    insert_ordered_list(NULL, elements, 0, 6);

    free_bst(t);
    printf("test_insert_ordered_list: PASS\n");
}

void test_bstprint() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    int vals[] = {10, 5, 15};
    for (int i = 0; i < 3; i++) bstadd(t, &vals[i]);

    printf("expected: [5, 10, 15] => ");
    bstprint(t, print_int);
    printf("expected: [] => ");
    bstprint(t, NULL);
    printf("expected: [] => ");
    bstprint(NULL, print_int);

    free_bst(t);
}

void test_free_bst() {
    bst *t = create_bst(copy_int, free_int, compare_int);
    int val = 10;
    bstadd(t, &val);
    free_bst(t);
    free_bst(NULL);
    printf("test_free_bst: PASS\n");
}

int main(void) {
    test_create_bst();
    test_bstadd();
    test_bstsearch();
    test_bstdelete();
    test_insert_ordered_list();
    test_bstprint();
    test_free_bst();

    printf("✅ All BST API tests passed!\n");
    return 0;
}

