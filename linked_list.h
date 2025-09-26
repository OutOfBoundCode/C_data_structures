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

void free_linked_list(linked_list *list, void (*free_value)(void*));

ssize_t get_node_index(const linked_list *list, void* element, int (*compare) (void*, void*)); 

void print_linked_list(const linked_list *list, void (*print_node) (void*)); 

void reverse_linked_list(linked_list *list);

node *get_node(const linked_list *list, ssize_t index);

int append_node(linked_list *list, void* element);  

int add_node(linked_list *list, ssize_t index, void* element); 

int pop_node(linked_list *list, void (*free_value)(void*));

int delete_node(linked_list *list, ssize_t index, void (*free_value)(void*));
