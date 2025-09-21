#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"


node* create_node(int value){
    node* node = malloc(sizeof(node));

    if (node==NULL) return NULL;
    
    node->value = value;
    node-> next = NULL;
    
    return node;
}

linked_list* create_linked_list(){
    linked_list* list = malloc(sizeof(linked_list));
    
    if (list==NULL) return NULL;
    
    list->head = NULL;
    list-> tail = NULL;
    list->length = 0;
    
    return list;
}

void free_list(linked_list* list){
    if (list==NULL) return;
    
    node* delete_pointer = list->head;
    node* temp_next; 
    while (delete_pointer != NULL){
        temp_next = delete_pointer->next;
        free(delete_pointer);
        delete_pointer=temp_next;
    }

    free(list);

}


ssize_t get_length(linked_list* list){
    return list->length;
}

ssize_t get_index(linked_list* list, int element){
    if (list==NULL) return -1;
    
    node* current = list->head;
    
    ssize_t current_index = 0;
     while (current != NULL) {
        if (current->value == element){
            return current_index;
        }        
        current = current->next;
        current_index++;
    }

    return -1;
}

void print_list(linked_list* list){
    if (list==NULL || list->head==NULL) 
    {
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
    if (list==NULL) return;
    if (list->head==NULL) return;
    
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

node* get_last_node(linked_list* list){
    if (list==NULL) return NULL;
    
    return list->tail;
}

node* get_pre_tail(linked_list* list){
    if (list==NULL) return NULL;
    if (list->head==NULL) return NULL;
    if (list->head==list->tail) return NULL;

    node* current = list->head;
    while (current->next != list->tail){
        current = current->next;
    }

    return current;
}

node* get_node_index(linked_list* list, int index){
    if (list==NULL) return NULL;
    if (index < 0) return NULL;
    if (index > (list->length-1)) {
        fprintf(stderr, "Index %d out of bounds", index);
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

void append_node(linked_list* list, int value){
    if (list==NULL) return;
    
    node* last_node = get_last_node(list);
    node* new_node = create_node(value);

    if (new_node==NULL) return;
    
    if (list->head==NULL){
        list->head=new_node;
        list->tail=new_node;
        list->length++;
        return;
    }
    
    last_node->next = new_node;
    list->tail=new_node;
    list->length++;
}

void add_node_first(linked_list* list, int value){
    
    if (list==NULL) return;
    
    node* newhead = create_node(value);
    
    if (newhead==NULL) return;
    
    if (list->head==NULL){
        list->head=newhead;
        list->tail=newhead;
        list->length++;
        return; 
    }
    newhead->next = list->head;
    list->head = newhead;
    list->length++;
}

void add_node_index(linked_list* list, int index, int value){
    if (list==NULL) return;
    
    if (index==0) {
        add_node_first(list, value);
        return;
    }
    
    node* prenode = get_node_index(list, index-1);
    
    if (prenode==NULL) return;
    
    node* newnode = create_node(value);

    if (newnode==NULL) return;

    node* postnode = prenode->next;
    prenode->next = newnode; 
    newnode->next = postnode;
    if (newnode->next == NULL) list->tail = newnode;
    list->length++;
}

void delete_first_node(linked_list* list){
    if (list==NULL) return;
    
    if (list->head==NULL) return;
    
    if (list->head==list->tail){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return;
    }
    
    node* old_head = list->head;
    list->head = list->head->next;
    free(old_head);
    list->length--;
}

void delete_last_node(linked_list* list){
    if (list==NULL) return;
    
    if (list->head==NULL) return;
    
    if (list->head==list->tail){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return;
    }
    
    node* old_tail = list->tail;
    list->tail = get_pre_tail(list);
    list->tail->next = NULL;
    free(old_tail);
    list->length--;
}

void delete_node_index(linked_list* list, int index){
    if (list==NULL) return;
    
    if (index==0) {
        delete_first_node(list);
        return;
    }
    
    node* prenode = get_node_index(list,index-1);
    
    if (prenode==NULL) return;
      
    if (prenode->next==list->tail){
        delete_last_node(list);
        return;
    }
    
    node* deleted_node = prenode->next;
    node* postnode = prenode->next->next;
    
    prenode->next = postnode;
    
    free(deleted_node);
    list->length--;
}
