#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "array_list.h"

array_list* create_array_list(ssize_t array_size){
    ssize_t size;
    // check if init_size is specified, if it <= 0 a default size of 10 is used
    if (array_size > 0) 
        size = array_size;
    else 
        size = 10;

    array_list* list = malloc(sizeof(array_list));

    if (list == NULL) return NULL;

    list->arr = malloc(sizeof(void*) * size);

    if (list->arr == NULL) return NULL;

    list->length = 0;
    list->max_size = size;

    return list;
}

void free_list(array_list *list, void (*free_value) (void*)){
    if (list == NULL) return;
    
    ssize_t current_index = 0;

    if (free_value != NULL){
       while (current_index < list->length){
            free_value(list->arr[current_index]);
            current_index++;
    }
    }
    free(list->arr);
    free(list);
}

ssize_t get_index(const array_list *list, void *element, int (*compare)(void *, void *)){
    if (list == NULL || element == NULL || compare == NULL) return -1;
   
    ssize_t current_index = 0;
    
    while (current_index < list->length){
        if (compare(list->arr[current_index], element) == 0)
            return current_index;

        current_index++;
    }

    return -1;
}

void print_list(const array_list *list, void (*print_element)(void *)){
   if (list == NULL || list->length == 0 || print_element == NULL) {
        printf("[]\n");
        return;
    }
    
    ssize_t current_index = 0;
    
    printf("[");
    
    while (current_index < list->length) {
        print_element(list->arr[current_index]);
        current_index++;
        printf(", ");
    }
    
    printf("\b\b]\n"); 
}

void swap(void** ptr1, void** ptr2){
    void* temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

void reverse_list(array_list *list){
    if (list == NULL) return; 
    
    ssize_t index1 = 0, index2 = list->length - 1; // set to pointing indexes to swap elements

    while (index1 < index2){
        swap(&list->arr[index1], &list->arr[index2]);
        index1++;
        index2--;
    }
}

void *get(const array_list *list, ssize_t index){
    if (list == NULL || index < 0 || index >= list->length) return NULL;
    return list->arr[index];
}

int resize_list(array_list* list){
    void **new_arr = realloc(list->arr, list->max_size * 2 * sizeof(void*)); // resizing the array to double the old size
    if (new_arr == NULL) return -1;
    list->arr = new_arr;  
    list->max_size *= 2;
    return 0;
}

int append(array_list *list, void* element){
    if (list == NULL || element == NULL) return -1;
    if (list->length >= list->max_size)
        if (resize_list(list) == -1) return -1; 

    list->arr[list->length] = element;
    list->length++;
    
    return 0;
}

int add(array_list *list, ssize_t index, void* element){
    if (list == NULL || element == NULL || index > list->length) return -1;
    if (list->length >= list->max_size)
        if (resize_list(list) == -1) return -1; 

    void* dest = &list->arr[index + 1];
    void* src = &list->arr[index];
    size_t copy_size = (list->length - index) * sizeof(void*); // calculating how many bytes to shift
    
    memmove(dest, src, copy_size);

    list->arr[index] = element;
    return 0;
}

int pop(array_list *list, void (*free_value)(void*)){
    if (list == NULL) return -1;
    if (list->length <= 0) return -1;

    if (free_value != NULL)
        free_value(list->arr[list->length-1]);

    list->length--;

    return 0;
}

int delete(array_list *list, ssize_t index, void (*free_value)(void*)){
    if (list == NULL || index >= list->length) return -1;
    
    if (free_value != NULL)
        free_value(list->arr[index]);

    void* dest = &list->arr[index];
    void* src = &list->arr[index + 1];
    size_t copy_size = ((list->length - 1) - index) * sizeof(void*); // calculating how many bytes to shift
   
    memmove(dest, src , copy_size);

    list->length--;

    return 0;
}
