#pragma once

struct node {
    int value;
    struct node* next;
};

typedef struct node node;

typedef struct linked_list {
    node* head;
    node* tail;
    size_t length;
} linked_list ;


node *create_node(int initvalue);

linked_list *create_linked_list();

void free_list(linked_list *list);

ssize_t get_index(const linked_list *list, int element);

void print_list(const linked_list *list);

void reverse_list(linked_list *list);

node *get_node_index(const linked_list *base, int index);

void append_node(linked_list *base, int value);

void add_node_index(linked_list *base, int index, int value);

void delete_last_node(linked_list *list);

void delete_node_index(linked_list *list, int index);
