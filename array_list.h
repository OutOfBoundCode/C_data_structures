#pragma once

typedef struct {
    ssize_t length;
    ssize_t max_size;
    ssize_t type_size;
    void** arr;
} array_list;

array_list* create_array_list(ssize_t array_size);

void free_array_list(array_list *list, void (*free_element) (void*));

ssize_t get_element_index(const array_list *list, void* element, int (*compare) (void*, void*)); 

void print_array_list(const array_list *list, void (*print_element) (void*)); 

void reverse_array_list(array_list *list);

void *get_element(const array_list *list, ssize_t index);

int append_element(array_list *list, void* element);  

int add_element(array_list *list, ssize_t index, void* element); 

int pop_element(array_list *list, void (*free_value)(void*));

int delete_element(array_list *list, ssize_t index, void (*free_value)(void*));
