#pragma once

struct dnode{
    int value;
    struct dnode* next;
    struct dnode* previous;

};

typedef struct dnode dnode;

typedef struct doubly_linked_list {
    dnode* head;
    dnode* tail;
    ssize_t length;

} doubly_linked_list ;


dnode *create_node(int initvalue);

doubly_linked_list *create_doubly_linked_list();

void print_list(doubly_linked_list *e);

void reverse_list(doubly_linked_list *list);

dnode *get_last_node(doubly_linked_list *base);

dnode *get_node_index(doubly_linked_list *base, int index);

void append_node(doubly_linked_list *base, int nextvalue);

void add_node_first(doubly_linked_list *base, int nextvalue);

void add_node_index(doubly_linked_list *base, int index, int nextvalue);

void delete_first_node(doubly_linked_list *list);

void delete_last_node(doubly_linked_list *list);

void delete_node_index(doubly_linked_list *list, int index);
