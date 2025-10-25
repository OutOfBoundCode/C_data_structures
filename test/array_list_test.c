#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include "../array_list.h"

// ----------------- Helper functions -----------------

void free_int(void *p) {
    free(p);
}

void *copy_int(void *p) {
    if (!p) return NULL;
    int *c = malloc(sizeof(int));
    if (!c) return NULL;
    *c = *(int*)p;
    return c;
}

int compare_int(void *a, void *b) {
    if (a == NULL || b == NULL) return -1;
    int ia = *(int*)a;
    int ib = *(int*)b;
    return (ia == ib) ? 0 : -1;
}

void print_int(void *p) {
    if (p == NULL) return;
    printf("%d", *(int*)p);
}

// ----------------- Tests -----------------

void test_create_list() {
    array_list *list = create_array_list(0, copy_int, free_int, compare_int);
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->max_size == 10);
    assert(list->arr != NULL);

    array_list *list2 = create_array_list(5, copy_int, free_int, compare_int);
    assert(list2 != NULL && list2->max_size == 5);

    free_array_list(list);
    free_array_list(list2);
    printf("create list test: PASS\n");
}

void test_free_list() {
    array_list *list = create_array_list(2, copy_int, free_int, compare_int);
    assert(list != NULL);
    free_array_list(list);

    list = create_array_list(2, copy_int, free_int, compare_int);
    assert(list != NULL);
    int v = 10;
    alappend(list, &v);
    free_array_list(list);

    list = create_array_list(2, copy_int, free_int, compare_int);
    assert(list != NULL);
    int x = 20, y = 21, z = 22;
    alappend(list, &x);
    alappend(list, &y);
    alappend(list, &z);
    free_array_list(list);

    free_array_list(NULL);

   printf("free list test: PASS\n");
}

void test_append() {
    array_list *list = create_array_list(2, copy_int, free_int, compare_int);
    assert(list != NULL);

    int v1 = 10;
    assert(alappend(list, &v1) == 0);
    assert(*(int*)alget(list, 0) == 10 && list->length == 1);

    int v2 = 20;
    assert(alappend(list, &v2) == 0);
    assert(*(int*)alget(list, 1) == 20 && list->length == 2);

    int v3 = 30;
    assert(alappend(list, &v3) == 0);
    assert(list->length == 3);
    assert(list->max_size >= 3);

    assert(alappend(list, NULL) == -1);

    int num = 100;
    assert(alappend(NULL, &num) == -1);

    free_array_list(list);
    printf("append test: PASS\n");
}

void test_get() {
    array_list *list = create_array_list(3, copy_int, free_int, compare_int);
    assert(list != NULL);

    assert(alget(list, 0) == NULL);

    int v1 = 10, v2 = 20, v3 = 30;
    alappend(list, &v1);
    alappend(list, &v2);
    alappend(list, &v3);

    int *val = alget(list, 0);
    assert(val && *val == 10);

    val = alget(list, list->length - 1);
    assert(val && *val == 30);

    val = alget(list, 1);
    assert(val && *val == 20);

    assert(alget(list, -1) == NULL);
    assert(alget(list, 3) == NULL);
    assert(alget(NULL, 0) == NULL);

    free_array_list(list);
    printf("get test: PASS\n");
}

void test_set() {
    array_list *list = create_array_list(3, copy_int, free_int, compare_int);
    assert(list != NULL);

    int number = 104;
    assert(alset(list, 0, &number) == -1);

    int v1 = 10, v2 = 20, v3 = 20;
    alappend(list, &v1);
    alappend(list, &v2);
    alappend(list, &v3);

    int n1 = 100;
    assert(alset(list, 0, &n1) == 0);
    assert(*(int*)alget(list, 0) == 100);

    int n2 = 200;
    assert(alset(list, list->length - 1, &n2) == 0);
    assert(*(int*)alget(list, list->length - 1) == 200);

    int n3 = 300;
    assert(alset(list, 1, &n3) == 0);
    assert(*(int*)alget(list, 1) == 300);

    assert(alset(list, -1, &number) == -1);
    assert(alset(list, 3, &number) == -1);
    assert(alset(NULL, 0, &number) == -1);
    assert(alset(list, 0, NULL) == -1);

    free_array_list(list);
    printf("set test: PASS\n");
}

void test_add() {
    array_list *list = create_array_list(2, copy_int, free_int, compare_int);
    assert(list != NULL);

    int number = 80;
    assert(aladd(list, 1, &number) == -1);

    int v0 = 70;
    assert(aladd(list, 0, &v0) == 0);

    int v1 = 10, v2 = 30;
    alappend(list, &v1);
    alappend(list, &v2);

    assert(list->max_size > 2);

    int vh = 5;
    assert(aladd(list, 0, &vh) == 0);
    assert(*(int*)alget(list, 0) == 5 && list->length == 4);

    int vm = 20;
    assert(aladd(list, 1, &vm) == 0);
    assert(*(int*)alget(list, 1) == 20 && list->length == 5);

    int vt = 40;
    assert(aladd(list, list->length, &vt) == 0);
    assert(*(int*)alget(list, list->length - 1) == 40);

    assert(aladd(list, -1, &number) == -1);
    assert(aladd(list, list->length + 1, &number) == -1);
    assert(aladd(NULL, 0, &number) == -1);
    assert(aladd(list, 1, NULL) == -1);

    free_array_list(list);
    printf("add test: PASS\n");
}

void test_pop() {
    array_list *list = create_array_list(3, copy_int, free_int, compare_int);
    assert(list != NULL);

    assert(alpop(list) == -1);

    int v1 = 10, v2 = 10, v3 = 10;
    alappend(list, &v1);
    alappend(list, &v2);
    alappend(list, &v3);

    assert(alpop(list) == 0);
    assert(list->length == 2);

    assert(alpop(list) == 0);
    assert(list->length == 1);

    assert(alpop(list) == 0);
    assert(list->length == 0);

    assert(alpop(NULL) == -1);

    char *t = "text";
    alappend(list, t);
    assert(alpop(list) == 0);

    free_array_list(list);
    printf("pop test: PASS\n");
}

void test_delete() {
    array_list *list = create_array_list(5, copy_int, free_int, compare_int);

    assert(aldelete(list, 0) == -1);

    int v1 = 10, v2 = 20, v3 = 30, v4 = 40;
    alappend(list, &v1);
    alappend(list, &v2);
    alappend(list, &v3);
    alappend(list, &v4);

    assert(aldelete(list, 0) == 0);
    assert(*(int*)alget(list, 0) == 20 && list->length == 3);

    assert(aldelete(list, 1) == 0);
    assert(*(int*)alget(list, 1) == 40 && list->length == 2);

    assert(aldelete(list, list->length - 1) == 0);
    assert(*(int*)alget(list, 0) == 20 && list->length == 1);

    assert(aldelete(list, 0) == 0);
    assert(list->length == 0);

    assert(aldelete(list, -1) == -1);
    assert(aldelete(list, 1) == -1);
    assert(aldelete(NULL, 0) == -1);

    char *t = "text";
    alappend(list, t);
    assert(aldelete(list, 0) == 0);

    free_array_list(list);
    printf("delete test: PASS\n");
}

void test_reverse() {
    array_list *list = create_array_list(5, copy_int, free_int, compare_int);
    assert(list != NULL);

    alreverse(list);

    int v = 40;
    alappend(list, &v);
    alreverse(list);
    assert(*(int*)alget(list, 0) == 40);

    alpop(list);

    for (int i = 1; i <= 5; i++) {
        alappend(list, &i);
    }

    alreverse(list);
    assert(*(int*)alget(list, 0) == 5);
    assert(*(int*)alget(list, 1) == 4);
    assert(*(int*)alget(list, 2) == 3);
    assert(*(int*)alget(list, 3) == 2);
    assert(*(int*)alget(list, 4) == 1);

    alreverse(list);
    assert(*(int*)alget(list, 0) == 1);
    assert(*(int*)alget(list, 1) == 2);
    assert(*(int*)alget(list, 2) == 3);
    assert(*(int*)alget(list, 3) == 4);
    assert(*(int*)alget(list, 4) == 5);

    alreverse(NULL);

    free_array_list(list);
    printf("reverse test: PASS\n");
}

void test_get_index() {
    array_list *list = create_array_list(3, copy_int, free_int, compare_int);
    assert(list != NULL);

    int a = 10, b = 20, c = 30;
    assert(alget_index(list, &a) == -1);

    alappend(list, &a);
    alappend(list, &b);
    alappend(list, &c);

    assert(alget_index(list, &a) == 0);
    assert(alget_index(list, &c) == 2);
    assert(alget_index(list, &b) == 1);

    int x = 99;
    assert(alget_index(list, &x) == -1);

    assert(alget_index(NULL, &a) == -1);
    assert(alget_index(list, NULL) == -1);

    free_array_list(list);
    printf("get_index test: PASS\n");
}

void test_print() {
    array_list *list = create_array_list(3, copy_int, free_int, compare_int);
    assert(list != NULL);

    printf("Expected [] => ");
    alprint(list, print_int);

    int v1 = 10, v2 = 20;
    alappend(list, &v1);
    alappend(list, &v2);

    printf("Expected [10, 20] => ");
    alprint(list, print_int);

    alpop(list);

    printf("Expected [10] => ");
    alprint(list, print_int);

    printf("Expected [] => ");
    alprint(NULL, print_int);

    printf("Expected [] => ");
    alprint(list, NULL);

    free_array_list(list);
    printf("print test: PASS\n");
}

void test_stress_operations() {
    array_list *list = create_array_list(1, copy_int, free_int, compare_int);

    const int N = 100;
    for (int i = 0; i < N; i++) {
        alappend(list, &i);
    }
    assert(list->length == N);

    alreverse(list);
    int *first = alget(list, 0);
    int *last = alget(list, list->length-1);
    assert(first && last);
    assert(*first == N-1 && *last == 0);

    for (int i = 0; i < 50; i++) {
        assert(aldelete(list, list->length/2) == 0);
    }

    for (int i = 0; i < 10; i++) {
        assert(alset(list, i, &i) == 0);
    }

    free_array_list(list);
    printf("stress test: PASS\n");
}

void test_resize_list(){
    array_list* list = create_array_list(4, copy_int, free_int, compare_int);
    assert(list != NULL);
    assert(list->max_size == 4);

    for (int i = 0; i < 5; i++){
        alappend(list, &i);
    }

    assert(list->max_size == 8);

    alpop(list);
    alpop(list);
    alpop(list);

    assert(list->max_size == 4);

    free_array_list(list);
    printf("resize test: PASS\n");
}

// ----------------- Main -----------------

int main(void) {
    test_create_list();
    test_free_list();
    test_append();
    test_get();
    test_set();
    test_add();
    test_pop();
    test_delete();
    test_reverse();
    test_get_index();
    test_print();
    test_resize_list();
    test_stress_operations();
    printf("✅ All array_list tests passed!\n");
    return 0;
}

