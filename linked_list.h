#pragma once

struct node {
    int value;
    struct node* next;
};

typedef struct node node;

typedef struct linked_list {
    node* head;
    node* tail;
    ssize_t length;
} linked_list ;


node *create_node(int initvalue);

linked_list *create_linked_list();

void print_list(linked_list *e);

void reverse_list(linked_list *list);

node *get_last_node(linked_list *base);

node *get_node_index(linked_list *base, int index);

void append_node(linked_list *base, int nextvalue);

void add_node_first(linked_list *base, int nextvalue);

void add_node_index(linked_list *base, int index, int nextvalue);

void delete_first_node(linked_list *list);

void delete_last_node(linked_list *list);

void delete_node_index(linked_list *list, int index);

node *get_pre_tail(linked_list *list);

void free_list(linked_list *list);

ssize_t get_length(linked_list *list);

ssize_t get_index(linked_list *list, int value);
