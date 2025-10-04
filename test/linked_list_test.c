#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h> 
#include "../linked_list.h"

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
    linked_list *list = create_linked_list();

    // check if the list is created successfully with the expected values
    assert(list != NULL);
    assert(list->length == 0);
    assert(list->head == NULL);
    assert(list->tail == NULL);

    printf("create list test: PASS\n");

    free_linked_list(list, free_int);
}

void test_free_list(){
    linked_list *list = create_linked_list();
    assert(list != NULL);

    // free empty list
    free_linked_list(list, free_int);

    list = create_linked_list();
    assert(list != NULL);
    llappend(list, make_element_int(23));
    
    // free a list with a single element
    free_linked_list(list, free_int);

    list = create_linked_list();
    assert(list != NULL);
    llappend(list, make_element_int(23));
    llappend(list, make_element_int(33));
    llappend(list, make_element_int(54));
    llappend(list, make_element_int(67));
    
    // free a list with multiple elementst
    free_linked_list(list, free_int);

    list = create_linked_list();    
    assert(list != NULL);
    llappend(list, "text1"); // append a pointer to a constant (the list doesn't own the memory)
    llappend(list, "text2");

    // free a list that doesn't own the memory, we pass null
    free_linked_list(list, NULL);

    // free a NULL pointer
    free_linked_list(NULL, free_int);

    printf("free list test: PASS\n");
}

void test_get_index(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    int *head_number = make_element_int(20);
    int *tail_number = make_element_int(30);
    int *mid_number = make_element_int(33);
    int *non_existed_number = make_element_int(90);

    ssize_t index = llget_index(list, head_number, compare_int); // get index from empty list (should return -1)
    assert(index == -1);

    llappend(list, make_element_int(20));
    llappend(list, make_element_int(23));
    llappend(list, make_element_int(33));
    llappend(list, make_element_int(27));
    llappend(list, make_element_int(30));

    index = llget_index(list, head_number, compare_int); // search at the head
    assert(index == 0);

    index = llget_index(list, tail_number, compare_int); // search at the tail
    assert(index == 4);

    index = llget_index(list, mid_number, compare_int); // search at the middle
    assert(index == 2);

    index = llget_index(list, non_existed_number, compare_int); // search for an element that isn't in the list (should return -1)
    assert(index == -1);

    index = llget_index(list, head_number, NULL); // pass NULL for compare (should fail and return -1)
    assert(index == -1);
    
    index = llget_index(NULL, mid_number, compare_int); // pass NULL list (should fail and return -1)
    assert(index == -1);

    index = llget_index(list, NULL, compare_int); // pass NULL element (should fail and return -1)
    assert(index == -1);

    // clean up
    free_int(head_number);
    free_int(tail_number);
    free_int(mid_number);
    free_int(non_existed_number);

    free_linked_list(list, free_int);

    printf("test get_index: PASS\n");
} 

void test_print_list(){
    linked_list* list = create_linked_list();
    assert(list != NULL);
    
    printf("Expected [] => ");
    llprint(list, print_int); // print empty list (should print [])
    
    llappend(list, make_element_int(2));

    printf("Expected [2] => ");
    llprint(list, print_int); // print list with one element (should print [2])

    llappend(list, make_element_int(3));
    llappend(list, make_element_int(6));

    printf("Expected [2, 3, 6] => ");
    llprint(list, print_int); // print list with multiple elements (should print [2, 3, 6])

    printf("Expected [] => ");
    llprint(NULL, print_int); // pass NULL for list => should fail and print []

    printf("Expected [] => ");
    llprint(list, NULL); // pass NULL for print_element => should fail and print []
    
    free_linked_list(list, free_int);
}

void test_reverse(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    llreverse(list); // reverse empty list 

    llappend(list, make_element_int(20));

    llreverse(list); // reverse list with single element

    int number = *(int*)llget(list, 0);

    assert(number == 20);

    llappend(list, make_element_int(30));
    llappend(list, make_element_int(40));
    llappend(list, make_element_int(50));
    llappend(list, make_element_int(60));

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
    
    free_linked_list(list, free_int);

    printf("reverse list test: PASS\n");
}

void test_get(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    int* num_ptr = llget(list, 0); // get element from empty list (should return NULL)
    assert(num_ptr == NULL);

    llappend(list, make_element_int(30));
    llappend(list, make_element_int(40));
    llappend(list, make_element_int(50));

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

    free_linked_list(list, free_int);

    printf("get test: PASS\n");
}

void test_set(){
    linked_list *list = create_linked_list();
    assert(list != NULL);

    int* number_ptr = make_element_int(20);

    assert(llset(list, 0, number_ptr, free_int) == -1); // empty list (should fail and return -1)

    llappend(list, make_element_int(10));
    llappend(list, make_element_int(20));
    llappend(list, make_element_int(30));

    assert(llset(list, 0, make_element_int(55), free_int) == 0); // set the head
    assert(*(int*)llget(list, 0) == 55);
    
    assert(llset(list, 1, make_element_int(100), free_int) == 0); // set the middle
    assert(*(int*)llget(list, 1) == 100);

    assert(llset(list, 2, make_element_int(500), free_int) == 0); // set the tail
    assert(*(int*)llget(list, 2) == 500);
    
    assert(llset(list, -1, number_ptr, free_int) == -1); // negative index
    assert(llset(list, 3, number_ptr, free_int) == -1); // out of bound index

    free_int(number_ptr);
    free_linked_list(list, free_int);
    printf("set test: PASS\n");
}

void test_append(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    assert(llappend(list, make_element_int(20)) == 0); // append to empty list
    assert(*(int*)llget(list, 0) == 20 && list->length == 1);
    
    assert(llappend(list, make_element_int(30)) == 0); // append to single list element
    assert(*(int*)llget(list, 1) == 30 && list->length == 2);

    // append multiple elements
    assert(llappend(list, make_element_int(40)) == 0);
    assert(*(int*)llget(list, 2) == 40 && list->length == 3);

    assert(llappend(list, make_element_int(50)) == 0);
    assert(*(int*)llget(list, 3) == 50 && list->length == 4);

    assert(llappend(list, make_element_int(60)) == 0 && list->length == 5);
    assert(*(int*)llget(list, 4) == 60);

    assert(llappend(list, NULL) == -1); // appending NULL, should refuse and return -1

    int *number_ptr = make_element_int(100); 
    assert(llappend(NULL, number_ptr) == -1); // passing NULL list, should fail and return -1

    free_int(number_ptr);

    free_linked_list(list, free_int);

    printf("append test: PASS\n");
}

void test_add(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    int* number_ptr = make_element_int(30);
    assert(lladd(list, 1, number_ptr) == -1); // add to empty list at non-zero index (should fail and return -1)

    assert(lladd(list, 0, make_element_int(20)) == 0); // add to empty list at 0 (should work and return 0)

    assert(*(int*)llget(list, 0) == 20 && list->length == 1);

    assert(lladd(list, 0, make_element_int(30)) == 0); // add at head
    
    assert(*(int*)llget(list, 0) == 30 && *(int*)list->head->value == 30 && list->length == 2);

    assert(lladd(list, list->length, make_element_int(50)) == 0); // add at tail 
    
    assert(*(int*)llget(list, list->length -1) == 50 && *(int*)list->tail->value == 50 && list->length == 3);

    assert(lladd(list, 1, make_element_int(70)) == 0); // add at the middle
    assert(*(int*)llget(list, 1) == 70 && list->length == 4);

    assert(lladd(list, 5, number_ptr) == -1); // out of bound index (should fail and return -1)
    
    assert(lladd(list, -1, number_ptr) == -1); // negative index (should fail and return -1)

    assert(lladd(list, 1, NULL) == -1); // NULL element (should refuse and return -1)
    
    assert(lladd(NULL, 1, number_ptr) == -1); // NULL list (should fail and return -1)
    
    free_int(number_ptr);
    free_linked_list(list, free_int);

    printf("add test: PASS\n");
}

void test_pop(){
    linked_list *list = create_linked_list();
    assert(list != NULL);

    assert(llpop(list, free_int) == -1); // pop from empty list, should fail and return -1

    llappend(list, make_element_int(20));

    assert(llpop(NULL, free_int) == -1); // pop NULL list, should fail and return -1
    
    assert(list->length == 1);

    assert(llpop(list, free_int) == 0); // pop single-element list, the list should become empty

    assert(list->length == 0 && list->head == NULL && list->tail == NULL);

    llappend(list, make_element_int(80)); // length = 1
    llappend(list, make_element_int(50)); // length = 2
    
    assert(llpop(list, free_int) == 0); 
    assert(list->length == 1);

    assert(llpop(list, free_int) == 0); 
    assert(list->length == 0);

    llappend(list, "text"); // append pointer to read-only memory

    assert(llpop(list, NULL) == 0); // pop pointer to memory not owned by the list, should pass NULL to avoid freeing the memory
    
    free_linked_list(list, free_int);

    printf("pop test: PASS\n");
}

void test_delete(){
    linked_list* list = create_linked_list();
    assert(list != NULL);

    assert(lldelete(list, 0, free_int) == -1); // delete from empty list, should fail and return -1
    
    llappend(list, make_element_int(20));
    llappend(list, make_element_int(30));
    llappend(list, make_element_int(40));
    llappend(list, make_element_int(50));
    llappend(list, make_element_int(60));

    assert(lldelete(list, 0, free_int) == 0); // delete head 
    assert(*(int*)list->head->value == 30 && list->length == 4);
    
    assert(lldelete(list, list->length - 1, free_int) == 0); // delete tail
    assert(*(int*)list->tail->value == 50 && list->length == 3);
    
    assert(lldelete(list, 1, free_int) == 0); // delete middle
    assert(list->length == 2);
    
    assert(lldelete(list, -1, free_int) == -1); // negative index (should fail and return -1) 
    assert(list->length == 2);
 
    assert(lldelete(list, 2, free_int) == -1); // out of bound index (should fail and return -1) 
    assert(list->length == 2);
    
    assert(lldelete(list, 0, free_int) == 0);
    
    assert(lldelete(list, 0, free_int) == 0);

    assert(list->length == 0 && list->head == NULL && list->tail == NULL);

    llappend(list, "text");

    assert(lldelete(list, 0, NULL) == 0); // pass null to not free memory not owned by the list

    free_linked_list(list, free_int);

    printf("delete test: PASS\n");
}

void test_stress_operations() {
    linked_list *list = create_linked_list();
    assert(list != NULL);

    const int N = 100;
    for (int i = 0; i < N; ++i) {
        assert(llappend(list, make_element_int(i)) == 0);
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
        assert(lldelete(list, 10, free_int) == 0);
    }

    // set a few values
    for (int i = 0; i < 5; ++i) {
        assert(llset(list, i, make_element_int(i * 1000), free_int) == 0);
    }

    free_linked_list(list, free_int);

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

