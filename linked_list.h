#pragma once

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

void free_list(linked_list *list, void (*free_value)(void*));

ssize_t get_index(const linked_list *list, void* element, int (*compare) (void*, void*)); 

void print_list(const linked_list *list, void (*print_node) (void*)); 

void reverse_list(linked_list *list);

node *get(const linked_list *list, ssize_t index);

int append(linked_list *list, void* element);  

int add(linked_list *list, ssize_t index, void* element); 

int pop(linked_list *list, void (*free_value)(void*));

int delete(linked_list *list, ssize_t index, void (*free_value)(void*));
