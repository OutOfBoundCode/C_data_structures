#pragma once

#include <sys/types.h>

struct node {
    void* value;
    struct node* next;
};

typedef struct node node;

typedef struct linked_list {
    node* head;
    node* tail;
    ssize_t length;
} linked_list ;


node *create_node(void* element); 

linked_list *create_linked_list();

int free_linked_list(linked_list *list, void (*free_element)(void*));

ssize_t llget_index(const linked_list *list, void* element, int (*compare) (void*, void*)); 

void llprint(const linked_list *list, void (*print_node) (void*)); 

void llreverse(linked_list *list);

node *llget_node(const linked_list *list, ssize_t index);

void *llget(const linked_list *list, ssize_t index);

int llset(linked_list *list, ssize_t index, void* element, void (*free_element) (void*));

int llappend(linked_list *list, void* element);  

int lladd(linked_list *list, ssize_t index, void* element); 

int llpop(linked_list *list, void (*free_element)(void*));

int lldelete(linked_list *list, ssize_t index, void (*free_element)(void*));
