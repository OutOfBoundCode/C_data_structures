#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"


node* create_node(int value){
    node* node = malloc(sizeof(node));
    
    if (node == NULL) return NULL;
    
    node->value = value;
    node->next = NULL;
    
    return node;
}

linked_list* create_linked_list(){
    linked_list*  list = malloc(sizeof(linked_list));
    
    if (list == NULL) return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    
    return list;
}

void free_list(linked_list* list){
    if (list == NULL) return;
    
    node* delete_pointer = list->head;
    node* temp_next; 
    
    while (delete_pointer != NULL){

        temp_next = delete_pointer->next;
        free(delete_pointer);
        delete_pointer = temp_next;
    }

    free(list);
}



ssize_t get_index(const linked_list* list, int element){
    if (list == NULL) return -1;
    
    node* current = list->head;
    ssize_t current_index = 0;
    
    while (current != NULL) {
        if (current->value == element) return current_index;
                
        current = current->next;
        current_index++;
    }

    return -1;
}

void print_list(const linked_list* list){
    if (list == NULL || list->head == NULL) {
        printf("[]\n");
        return;
    }
    
    node* current = list->head;
    
    printf("[");
    
    while (current->next != NULL) {
        printf("%d, ", current->value);
        current = current->next;
    }
    
    printf("%d]\n",current->value);
}

void reverse_list(linked_list* list){
    if (list == NULL) return;
    if (list->head == NULL) return;
    
    node* current = list->head;
    node* pre = NULL;
    node* next = current->next;

     while (current != NULL) {
        next = current->next;  
        current->next = pre;   
        pre = current;         
        current = next;        
    }

    node* temp_head = list->head;
    
    list->head = list->tail;
    list->tail = temp_head;
}

node* get_node_index(const linked_list* list, int index){
    if (list == NULL) return NULL;
    if (index < 0) return NULL;
    if (index >= list->length) {
        fprintf(stderr, "Index %d out of bounds\n", index);
        return NULL;
    }
    
    node* current = list->head;
    int current_index = 0;

     while (current != NULL && current_index < index) {
        current = current->next;
        current_index++;
    }

    return current;
}

void append_node(linked_list*  list, int value){
    add_node_index(list, list->length, value);
}


void add_node_index(linked_list*  list, int index, int value){
    if (list == NULL) return;

    if (index < 0 || index > list->length) {
        fprintf(stderr, "Index %d out of bounds\n", index);
        return;
    }

    node* newnode = create_node(value);

    if (newnode == NULL) return;

    if (list->length == 0){
        list->head = newnode;
        list->tail = newnode;
        list->length++;   
        
        return;
    }

    if (index == 0) {
        newnode->next = list->head;
        list->head = newnode;
        list->length++;
       
        return;
    }
   
    if (index == list->length){
        list->tail->next = newnode;
        list->tail = newnode;
        list->length++;
        
        return;
    }

    node*  prenode = get_node_index(list, index-1);
    
    if (prenode == NULL) return;
    
    node*  postnode = prenode->next;
    
    prenode->next = newnode; 
    newnode->next = postnode;
    
    list->length++;
}

void delete_last_node(linked_list*  list){
    delete_node_index(list, list->length-1);
}

void delete_node_index(linked_list*  list, int index){
    if (list == NULL) return;
   
    if (index < 0 || index >= list->length) {
        fprintf(stderr, "Index %d out of bounds\n", index);
        return;
    }
    
    // check if the list one element
    if (list->length == 1 && index == 0) {   
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return;
    }

    // handle head deletion
    if (index == 0){
        node* oldhead = list->head;
        list->head = list->head->next;
        free(oldhead);
        list->length--;
        return;
    }
    
    // handle tail deletion
    if (index == list->length-1){
        node* old_tail = list->tail; 
        node* pretail = get_node_index(list, list->length-2); // lengt-2 to get the pre-tail node
        if (pretail==NULL) return;
        list->tail = pretail;
        list->tail->next = NULL;
        free(old_tail);
        list->length--;
        return;
    }

    node* prenode = get_node_index(list,index-1);
    
    if (prenode == NULL) return;
      
    node* deleted_node = prenode->next;
    node* postnode = prenode->next->next;
    
    prenode->next = postnode;
    free(deleted_node);
    list->length--;
}
