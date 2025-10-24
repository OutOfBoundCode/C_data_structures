#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h> 
#include "../linked_list.h"

// ----------------- Helper functions -----------------

void *copy_int(void* num) {
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = *(int*)num;
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
    linked_list *list = create_linked_list(copy_int, free_int, compare_int);

    // check if the list is created successfully with the expected values
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->compare == compare_int);
    assert(list->cpy == copy_int);
    assert(list->free_element == free_int);

    printf("create list test: PASS\n");

    free_linked_list(list);
}

void test_free_list(){
    linked_list *list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 20;

    // free empty list
    free_linked_list(list);

    list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    llappend(list, &test_number); // a pointer to a copy of the test_number would be add
    
    // free a list with a single element
    free_linked_list(list);

    list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    llappend(list, &test_number);
    llappend(list, &test_number);
    llappend(list, &test_number);
    llappend(list, &test_number);
    
    // free a list with multiple elementst
    free_linked_list(list);

    // free a NULL pointer
    free_linked_list(NULL);

    printf("free list test: PASS\n");
}

void test_get_index(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int head_number = 20;
    int tail_number = 30;
    int mid_number = 40;
    int non_existed_number = 100;

    ssize_t index = llget_index(list, &head_number); // get index from empty list (should return -1)
    assert(index == -1);

    llappend(list, &head_number);
    llappend(list, &mid_number);
    llappend(list, &tail_number);

    index = llget_index(list, &head_number); // search at the head
    assert(index == 0);

    index = llget_index(list, &tail_number); // search at the tail
    assert(index == 2);

    index = llget_index(list, &mid_number); // search at the middle
    assert(index == 1);

    index = llget_index(list, &non_existed_number); // search for an element that isn't in the list (should return -1)
    assert(index == -1);
  
    index = llget_index(NULL, &mid_number); // pass NULL list (should fail and return -1)
    assert(index == -1);

    index = llget_index(list, NULL); // pass NULL element (should fail and return -1)
    assert(index == -1);

    // clean up
    free_linked_list(list);

    printf("test get_index: PASS\n");
} 

void test_print_list(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    
    int test_number = 2;

    printf("Expected [] => ");
    llprint(list, print_int); // print empty list (should print [])
    
    llappend(list, &test_number);

    printf("Expected [2] => ");
    llprint(list, print_int); // print list with one element (should print [2])

    test_number = 3;
    llappend(list, &test_number);

    test_number = 6;
    llappend(list, &test_number);

    printf("Expected [2, 3, 6] => ");
    llprint(list, print_int); // print list with multiple elements (should print [2, 3, 6])

    printf("Expected [] => ");
    llprint(NULL, print_int); // pass NULL for list => should fail and print []

    printf("Expected [] => ");
    llprint(list, NULL); // pass NULL for print_element => should fail and print []
    
    free_linked_list(list);
}

void test_reverse(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 20;

    llreverse(list); // reverse empty list 

    llappend(list, &test_number);

    llreverse(list); // reverse list with single element

    int number = *(int*)llget(list, 0);

    assert(number == 20);

    test_number = 30;
    llappend(list, &test_number);
    test_number = 40;
    llappend(list, &test_number);
    test_number = 50;
    llappend(list, &test_number);
    test_number = 60;
    llappend(list, &test_number);

    // [20, 30, 40, 50, 60]
    llreverse(list); // should be [60, 50, 40, 30, 20], head = 60, tail = 20

    int head_number = *(int*)llget(list, 0);
    int num2 = *(int*)llget(list, 1);
    int num3 = *(int*)llget(list, 2);
    int num4 = *(int*)llget(list, 3);
    int tail_number = *(int*)llget(list, 4);

    assert(head_number == 60 && tail_number == 20 && *(int*)list->head->value == head_number && *(int*)list->tail->value == tail_number);
    
    assert(num2 == 50 && num3 == 40 && num4 == 30);

    llreverse(list); // should return the original order [20, 30, 40, 50, 60]
    
    head_number = *(int*)llget(list, 0);
    num2 = *(int*)llget(list, 1);
    num3 = *(int*)llget(list, 2);
    num4 = *(int*)llget(list, 3);
    tail_number = *(int*)llget(list, 4);

    assert(head_number == 20 && tail_number == 60 && *(int*)list->head->value == head_number && *(int*)list->tail->value == tail_number);

    assert(num2 == 30 && num3 == 40 && num4 == 50);

    llreverse(NULL); // reverse NULL list
    
    free_linked_list(list);

    printf("reverse list test: PASS\n");
}

void test_get(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 30;

    int* num_ptr = llget(list, 0); // get element from empty list (should return NULL)
    assert(num_ptr == NULL);

    llappend(list, &test_number);
    test_number = 40;
    llappend(list, &test_number);
    test_number = 50;
    llappend(list, &test_number);

    num_ptr = llget(list, 0); // get at the head

    assert(*num_ptr == 30);

    num_ptr = llget(list, 2); // get at the tail
    
    assert(*num_ptr == 50);

    num_ptr = llget(list, 1); // get at the middle

    assert(*num_ptr == 40);

    num_ptr = llget(list, -1); // invalid index (should fail and return NULL)

    assert(num_ptr == NULL);

    num_ptr = llget(list, 3); // out of bound index (should fail and return NULL)

    assert(num_ptr == NULL);
    
    num_ptr = llget(NULL, 1); // pass NULL list (should fail and return NULL)

    assert(num_ptr == NULL);

    free_linked_list(list);

    printf("get test: PASS\n");
}

void test_set(){
    linked_list *list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 20;

    assert(llset(list, 0, &test_number)== -1); // empty list (should fail and return -1)

    llappend(list, &test_number);
    test_number = 30;
    llappend(list, &test_number);
    test_number = 40;
    llappend(list, &test_number);

    test_number = 55;
    assert(llset(list, 0, &test_number)== 0); // set the head
    assert(*(int*)llget(list, 0) == 55);
    
    test_number = 100;
    assert(llset(list, 1, &test_number)== 0); // set the middle
    assert(*(int*)llget(list, 1) == 100);

    test_number = 500;
    assert(llset(list, 2, &test_number)== 0); // set the tail
    assert(*(int*)llget(list, 2) == 500);
    
    assert(llset(list, -1, &test_number)== -1); // negative index
    assert(llset(list, 3, &test_number)== -1); // out of bound index

    free_linked_list(list);
    printf("set test: PASS\n");
}

void test_append(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    
    int test_number = 20;

    assert(llappend(list, &test_number) == 0); // append to empty list
    assert(*(int*)llget(list, 0) == 20 && list->length == 1);
    
    test_number = 30;
    assert(llappend(list, &test_number) == 0); // append to single list element
    assert(*(int*)llget(list, 1) == 30 && list->length == 2);

    // append multiple elements

    test_number = 40;
    assert(llappend(list, &test_number) == 0);
    assert(*(int*)llget(list, 2) == 40 && list->length == 3);

    test_number = 50;
    assert(llappend(list, &test_number) == 0);
    assert(*(int*)llget(list, 3) == 50 && list->length == 4);

    test_number = 60;
    assert(llappend(list, &test_number) == 0 && list->length == 5);
    assert(*(int*)llget(list, 4) == 60);

    assert(llappend(list, NULL) == -1); // appending NULL, should refuse and return -1

    assert(llappend(NULL, &test_number) == -1); // passing NULL list, should fail and return -1

    free_linked_list(list);

    printf("append test: PASS\n");
}

void test_add(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 20;
    assert(lladd(list, 1, &test_number) == -1); // add to empty list at non-zero index (should fail and return -1)

    assert(lladd(list, 0, &test_number) == 0); // add to empty list at 0 (should work and return 0)

    assert(*(int*)llget(list, 0) == 20 && list->length == 1);

    test_number = 30;
    assert(lladd(list, 0, &test_number) == 0); // add at head
    
    assert(*(int*)llget(list, 0) == 30 && *(int*)list->head->value == 30 && list->length == 2);

    test_number = 50;
    assert(lladd(list, list->length, &test_number) == 0); // add at tail 
    
    assert(*(int*)llget(list, list->length -1) == 50 && *(int*)list->tail->value == 50 && list->length == 3);

    test_number = 70;
    assert(lladd(list, 1, &test_number) == 0); // add at the middle
    assert(*(int*)llget(list, 1) == 70 && list->length == 4);

    assert(lladd(list, 5, &test_number) == -1); // out of bound index (should fail and return -1)
    
    assert(lladd(list, -1, &test_number) == -1); // negative index (should fail and return -1)

    assert(lladd(list, 1, NULL) == -1); // NULL element (should refuse and return -1)
    
    assert(lladd(NULL, 1, &test_number) == -1); // NULL list (should fail and return -1)
    
    free_linked_list(list);

    printf("add test: PASS\n");
}

void test_pop(){
    linked_list *list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    
    int test_number = 20;

    assert(llpop(list) == -1); // pop from empty list, should fail and return -1

    llappend(list, &test_number);

    assert(llpop(NULL) == -1); // pop NULL list, should fail and return -1
    
    assert(list->length == 1);

    assert(llpop(list) == 0); // pop single-element list, the list should become empty

    assert(list->length == 0 && list->head == NULL && list->tail == NULL);

    llappend(list, &test_number); // length = 1
    llappend(list, &test_number); // length = 2
    
    assert(llpop(list) == 0); 
    assert(list->length == 1);

    assert(llpop(list) == 0); 
    assert(list->length == 0);
   
    free_linked_list(list);

    printf("pop test: PASS\n");
}

void test_delete(){
    linked_list* list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);
    
    int test_number = 20;

    assert(lldelete(list, 0)== -1); // delete from empty list, should fail and return -1
    
    llappend(list, &test_number);
    test_number = 30;
    llappend(list, &test_number);
    test_number = 40;
    llappend(list, &test_number);
    test_number = 50;
    llappend(list, &test_number);
    test_number = 60;
    llappend(list, &test_number);

    assert(lldelete(list, 0)== 0); // delete head 
    assert(*(int*)list->head->value == 30 && list->length == 4);
    
    assert(lldelete(list, list->length - 1)== 0); // delete tail
    assert(*(int*)list->tail->value == 50 && list->length == 3);
    
    assert(lldelete(list, 1)== 0); // delete middle
    assert(list->length == 2);
    
    assert(lldelete(list, -1)== -1); // negative index (should fail and return -1) 
    assert(list->length == 2);
 
    assert(lldelete(list, 2)== -1); // out of bound index (should fail and return -1) 
    assert(list->length == 2);
    
    assert(lldelete(list, 0)== 0);
    
    assert(lldelete(list, 0)== 0);

    assert(list->length == 0 && list->head == NULL && list->tail == NULL);

    free_linked_list(list);

    printf("delete test: PASS\n");
}

void test_stress_operations() {
    linked_list *list = create_linked_list(copy_int, free_int, compare_int);
    assert(list != NULL);

    int test_number = 20;

    const int N = 100;
    for (int i = 0; i < N; ++i) {
        test_number = i;
        assert(llappend(list, &test_number) == 0);
    }
    assert(list->length == N);

    // reverse and check first/last
    llreverse(list);
    int *first = (int*)llget(list, 0);
    int *last = (int*)llget(list, list->length - 1);
    assert(first != NULL && last != NULL);
    assert(*first == N - 1 && *last == 0);

    // delete a middle range
    for (int i = 0; i < 10; ++i) {
        assert(lldelete(list, 10)== 0);
    }

    // set a few values
    for (int i = 0; i < 5; ++i) {
        assert(llset(list, i, &test_number)== 0);
    }

    free_linked_list(list);

    printf("stress test: PASS\n");
}


int main(void) {
    test_create_list();
    test_free_list();
    test_get_index();
    test_print_list();
    test_reverse();
    test_get();
    test_set();
    test_append();
    test_add();
    test_pop();
    test_delete();
    test_stress_operations();
    printf("✅ All linked_list tests passed!\n");
    return 0;
}

