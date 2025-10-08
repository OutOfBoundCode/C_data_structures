#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include "../array_list.h"

// ----------------- Helper functions -----------------

int *make_element_int(int v) {
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = v;
    return p;
}

void free_int(void *p) {
    free(p);
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
    array_list *list = create_array_list(0); // should default to 10
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->max_size == 10);
    assert(list->arr != NULL);

    array_list *list2 = create_array_list(5);
    assert(list2 != NULL && list2->max_size == 5);

    free_array_list(list, free_int);
    free_array_list(list2, free_int);
    printf("create list test: PASS\n");
}

void test_free_list() {
    array_list *list = create_array_list(2);
    assert(list != NULL);
    free_array_list(list, free_int); // free empty list

    list = create_array_list(2);
    assert(list != NULL);
    alappend(list, make_element_int(10));
    free_array_list(list, free_int); // free a list with single element list

    list = create_array_list(2);
    assert(list != NULL);
    alappend(list, make_element_int(20));
    alappend(list, make_element_int(20));
    alappend(list, make_element_int(20));
    free_array_list(list, free_int); // free a list with multiple elements

    // free NULL pointer
    free_array_list(NULL, free_int);

    // free list without freeing elements (not owned memory)
    list = create_array_list(2);
    assert(list != NULL);
    alappend(list, "text1");
    alappend(list, "text2");
    free_array_list(list, NULL); // pass free_element NULL to avoid freeing memory not owned by the list

    printf("free list test: PASS\n");
}

void test_append() {
    array_list *list = create_array_list(2);
    assert(list != NULL);

    assert(alappend(list, make_element_int(10)) == 0); // append to empty list
    assert(*(int*)alget(list, 0) == 10 && list->length == 1);

    assert(alappend(list, make_element_int(20)) == 0); // append to a list with single element
    assert(*(int*)alget(list, 1) == 20 && list->length == 2);

    // force resize
    assert(alappend(list, make_element_int(30)) == 0); 
    assert(list->length == 3);
    assert(list->max_size >= 3);

    // append NULL
    assert(alappend(list, NULL) == -1); // should refuse and return -1

    // append into NULL list
    int *num = make_element_int(100);
    assert(alappend(NULL, num) == -1); // should fail and return -1
    free_int(num);

    free_array_list(list, free_int);
    printf("append test: PASS\n");
}

void test_get() {
    array_list *list = create_array_list(3);
    assert(list != NULL);

    // get from empty list
    assert(alget(list, 0) == NULL);

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(20));
    alappend(list, make_element_int(30));

    int *val = alget(list, 0); // get element at index 0
    assert(val && *val == 10);

    val = alget(list, list->length - 1); // get element at the end
    assert(val && *val == 30);

    val = alget(list, 1); // get from the middle
    assert(val && *val == 20);

    // out of range
    assert(alget(list, -1) == NULL);
    assert(alget(list, 3) == NULL);

    // NULL list
    assert(alget(NULL, 0) == NULL);

    free_array_list(list, free_int);
    printf("get test: PASS\n");
}

void test_set() {
    array_list *list = create_array_list(3);
    assert(list != NULL);

    int* number = make_element_int(104);

    assert(alset(list, 0, number, free_int) == -1); // set in empty list (should fail and return -1)

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(20));
    alappend(list, make_element_int(20));

    // valid sets
    assert(alset(list, 0, make_element_int(100), free_int) == 0); // set at head
    assert(*(int*)alget(list, 0) == 100);

    assert(alset(list, list->length - 1, make_element_int(200), free_int) == 0); // set at end
    assert(*(int*)alget(list, list->length - 1) == 200);

    assert(alset(list, 1, make_element_int(300), free_int) == 0); // set at the middle
    assert(*(int*)alget(list, 1) == 300);

    // out of range (should fail and return -1)
    assert(alset(list, -1, number, free_int) == -1);
    assert(alset(list, 3, number, free_int) == -1);

    // NULL list (should fail and return -1)
    assert(alset(NULL, 0, number, free_int) == -1);

    // NULL element (should fail and return -1)
    assert(alset(list, 0, NULL, free_int));

    free(number);
    free_array_list(list, free_int);
    printf("set test: PASS\n");
}

void test_add() {
    array_list *list = create_array_list(2);
    assert(list != NULL);
    
    int* number = make_element_int(80);

    assert(aladd(list, 1, number) == -1); // add to an empty list at non-zero index (should fail and return -1)

    assert(aladd(list, 0, make_element_int(70)) == 0); // add to an empty list at 0 (should success and return 0)

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(30));

    assert(list->max_size > 2); // test the resize
    
    // add at head
    assert(aladd(list, 0, make_element_int(5)) == 0);
    assert(*(int*)alget(list, 0) == 5 && list->length == 4);
    
    // add in middle
    assert(aladd(list, 1, make_element_int(20)) == 0);
    assert(*(int*)alget(list, 1) == 20 && list->length == 5);

    // add at tail
    assert(aladd(list, list->length, make_element_int(40)) == 0);
    assert(*(int*)alget(list, list->length - 1) == 40);

    // invalid indices (should fail and return -1)
    assert(aladd(list, -1, number) == -1);
    assert(aladd(list, list->length + 1, number) == -1);

    // NULL list (should fail and return -1)
    assert(aladd(NULL, 0, number) == -1);

    // NULL element (should fail and return -1)
    assert(aladd(list, 1, NULL) == -1);

    free(number);
    free_array_list(list, free_int);
    printf("add test: PASS\n");
}

void test_pop() {
    array_list *list = create_array_list(3);
    assert(list != NULL);

    // empty pop (should fail and return -1)
    assert(alpop(list, free_int) == -1);

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(10));
    alappend(list, make_element_int(10));

    assert(alpop(list, free_int) == 0); // pop from list with multiple elements
    assert(list->length == 2);

    assert(alpop(list, free_int) == 0);
    assert(list->length == 1);

    assert(alpop(list, free_int) == 0); // pop from list with 1 element (list become empty)
    assert(list->length == 0);


    // pop from NULL (should fail and return -1)
    assert(alpop(NULL, free_int) == -1);

    // non-owned memory
    alappend(list, "text");
    assert(alpop(list, NULL) == 0); // pass free_element NULL to avoid freeing non-owned memory (this also applies to set() since we free the old value)

    free_array_list(list, free_int);
    printf("pop test: PASS\n");
}

void test_delete() {
    array_list *list = create_array_list(5);

    // delete from empty list
    assert(aldelete(list, 0, free_int) == -1); // should fail and return -1

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(20));
    alappend(list, make_element_int(30));
    alappend(list, make_element_int(40));

    // delete head [10, 20, 30, 40] => [20, 30, 40]
    assert(aldelete(list, 0, free_int) == 0);
    assert(*(int*)alget(list, 0) == 20 && list->length == 3);

    // delete middle [20, 30, 40] => [20, 40]
    assert(aldelete(list, 1, free_int) == 0);
    assert(*(int*)alget(list, 1) == 40 && list->length == 2);

    // delete last [20, 40] => [20]
    assert(aldelete(list, list->length - 1, free_int) == 0);
    assert(*(int*)alget(list, 0) == 20 && list->length == 1);
    
    // delete from single element list (list become empty)
    assert(aldelete(list, 0, free_int) == 0);
    assert(list->length == 0);

    // invalid cases
    assert(aldelete(list, -1, free_int) == -1);
    assert(aldelete(list, 1, free_int) == -1);
    assert(aldelete(NULL, 0, free_int) == -1);

    // non-owned memory
    alappend(list, "text");
    assert(aldelete(list, 0, NULL) == 0);

    free_array_list(list, free_int);
    printf("delete test: PASS\n");
}

void test_reverse() {
    array_list *list = create_array_list(5);
    assert(list != NULL);

    alreverse(list); // reverse empty list
    
    alappend(list, make_element_int(40)); 

    alreverse(list); // reverse a list with single element
    
    assert(*(int*)alget(list, 0) == 40);

    alpop(list, free_int);

    for (int i = 1; i <= 5; i++) {
        alappend(list, make_element_int(i));
    }

    alreverse(list);
    assert(*(int*)alget(list, 0) == 5);
    assert(*(int*)alget(list, 1) == 4);
    assert(*(int*)alget(list, 2) == 3);
    assert(*(int*)alget(list, 3) == 2);
    assert(*(int*)alget(list, 4) == 1);

    alreverse(list); // should return the original order
    assert(*(int*)alget(list, 0) == 1);
    assert(*(int*)alget(list, 1) == 2);
    assert(*(int*)alget(list, 2) == 3);
    assert(*(int*)alget(list, 3) == 4);
    assert(*(int*)alget(list, 4) == 5);

    // reverse NULL
    alreverse(NULL);

    free_array_list(list, free_int);
    printf("reverse test: PASS\n");
}

void test_get_index() {
    array_list *list = create_array_list(3);
    assert(list != NULL);

    int *a = make_element_int(10);
    int *b = make_element_int(20);
    int *c = make_element_int(30);

    assert(alget_index(list, a, compare_int) == -1); // empty list

    alappend(list, a);
    alappend(list, b);
    alappend(list, c);

    assert(alget_index(list, a, compare_int) == 0); // element at head
    assert(alget_index(list, c, compare_int) == 2); // element at the end
    assert(alget_index(list, b, compare_int) == 1); // element at the middle

    int x = 99;
    assert(alget_index(list, &x, compare_int) == -1); // element doesn't exist (should fail and return -1)

    // invalid
    assert(alget_index(NULL, a, compare_int) == -1); // NULL list (should fail and return -1)
    assert(alget_index(list, NULL, compare_int) == -1); // NULL element (should fail and return -1)
    assert(alget_index(list, a, NULL) == -1); // compare is NULL (should fail and return -1)

    free_array_list(list, free_int);
    printf("get_index test: PASS\n");
}

void test_print() {
    array_list *list = create_array_list(3);
    assert(list != NULL);
    
    // print empty list
    printf("Expected [] => ");
    alprint(list, print_int);

    alappend(list, make_element_int(10));
    alappend(list, make_element_int(20));

    // print list with multiple elements
    printf("Expected [10, 20] => ");
    alprint(list, print_int);

    alpop(list, free_int);

    // print single element list
    printf("Expected [10] => ");
    alprint(list, print_int);

    // print NULL list
    printf("Expected [] => ");
    alprint(NULL, print_int);

    // print_element is NULL
    printf("Expected [] => ");
    alprint(list, NULL);

    free_array_list(list, free_int);
    printf("print test: PASS\n");
}

void test_stress_operations() {
    array_list *list = create_array_list(1);

    const int N = 100;
    for (int i = 0; i < N; i++) {
        assert(alappend(list, make_element_int(i)) == 0);
    }
    assert(list->length == N);

    // reverse and check
    alreverse(list);
    int *first = alget(list, 0);
    int *last = alget(list, list->length-1);
    assert(first && last);
    assert(*first == N-1 && *last == 0);

    // delete 50 items from middle
    for (int i = 0; i < 50; i++) {
        assert(aldelete(list, list->length/2, free_int) == 0);
    }

    // set values
    for (int i = 0; i < 10; i++) {
        assert(alset(list, i, make_element_int(i*100), free_int) == 0);
    }

    free_array_list(list, free_int);
    printf("stress test: PASS\n");
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
    test_stress_operations();
    printf("✅ All array_list tests passed!\n");
    return 0;
}

