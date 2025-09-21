#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"


dnode* create_node(int value){
    dnode* node = malloc(sizeof(dnode));

    if (node==NULL) return NULL;
    
    node->value = value;
    node-> next = NULL;
    node-> previous = NULL;
    
    return node;
}

doubly_linked_list* create_doubly_linked_list(){
    doubly_linked_list* list = malloc(sizeof(doubly_linked_list));
    
    if (list==NULL) return NULL;
    
    list->head = NULL;
    list-> tail = NULL;
    list->length = 0;

    return list;
}

ssize_t get_length(doubly_linked_list* list){
    if (list == NULL) return -1; 
    return list->length;
}

void print_list(doubly_linked_list* list){
    if (list==NULL || list->head==NULL) 
    {
        printf("[]\n");
        return;
    }
    dnode* current = list->head;

    printf("[");
    while (current->next != NULL) {
        printf("%d, ", current->value);
        current = current->next;
    }
    printf("%d]\n",current->value);
}

void reverse_list(doubly_linked_list* list){
    if (list==NULL) return;
    
    dnode* current = list->head;
    while (current != NULL) {
        dnode* temp_next = current->next;
        current->next = current->previous;
        current->previous = temp_next;
        current = temp_next;
    }
    dnode* temp_head = list->head;
    list->head = list->tail;
    list->tail = temp_head;
}

dnode* get_last_node(doubly_linked_list* list){
    if (list==NULL) return NULL;
    
    return list->tail;
}

dnode* get_node_index(doubly_linked_list* list, int index){
    if (list==NULL) return NULL;
    if (index < 0) return NULL;
    if (index > (list->length-1)) {
        fprintf(stderr, "Index %d out of bounds", index);
        return NULL;
    }
    
    
    dnode* current = list->head;
    
     int current_index = 0;
     while (current != NULL && current_index < index) {
        current = current->next;
        current_index++;
    }
    return current;
}

void append_node(doubly_linked_list* list, int value){
    if (list==NULL) return;
    
    dnode* last_node = get_last_node(list);
    dnode* new_node = create_node(value);

    if (new_node==NULL) return;
    
    if (list->head==NULL){
        list->head=new_node;
        list->tail=new_node;
        list->length++;
        return;
    }

    last_node->next = new_node;
    new_node->previous = last_node;
    list->tail=new_node;
    list->length++;
}

void add_node_first(doubly_linked_list* list, int value){

    if (list==NULL) return;
    
    dnode* newhead = create_node(value);

    if (newhead==NULL) return;

    if (list->head==NULL){
       list->head=newhead;
       list->tail=newhead;
       list->length++;
       return; 
    }
    newhead->next = list->head;
    list->head->previous = newhead ;
    list->head = newhead;
    list->length++;

}

void add_node_index(doubly_linked_list* list, int index, int value){
    if (list==NULL) return;
    
    if (index == list->length){
        append_node(list, value);
        return;
    }


    dnode* nnode = get_node_index(list, index);
    
    if (nnode==NULL) return;

        if (nnode==list->head){
        add_node_first(list, value);
        return;
    }
    
    dnode* newnode = create_node(value);

    if (newnode==NULL) return;

    nnode->previous->next = newnode; 
    newnode->previous = nnode->previous; 
    newnode->next = nnode;
    nnode->previous = newnode;
    list->length++;

}

void delete_first_node(doubly_linked_list* list){
    if (list==NULL) return;

    if (list->head==NULL) return;

    if (list->head==list->tail){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return;
    }

    dnode* old_head = list->head;
    list->head = list->head->next;
    list->head->previous = NULL;
    free(old_head);
    list->length--;

}

void delete_last_node(doubly_linked_list* list){
    if (list==NULL) return;
    
    if (list->head==NULL) return;
    
    if (list->head==list->tail){
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return;
    }
    
    dnode* old_tail = list->tail;
    list->tail = list->tail->previous;
    list->tail->next = NULL;
    free(old_tail);
    list->length--;
}

void delete_node_index(doubly_linked_list* list, int index){
    if (list==NULL) return;
    dnode* deleted_node = get_node_index(list,index);

    if (deleted_node==NULL) return;

    if (deleted_node==list->head){
        delete_first_node(list);
        return;
    }
    
    if (deleted_node==list->tail){
        delete_last_node(list);
        return;
    }

    dnode* pre = deleted_node->previous;
    dnode* next = deleted_node->next;

    pre->next = next;
    next->previous = pre;

    free(deleted_node);
    list->length--;

}

void free_list(doubly_linked_list* list){
    if (list==NULL) return;
    
    dnode* delete_pointer = list->head;
    dnode* temp_next; 
    while (delete_pointer != NULL){
        temp_next = delete_pointer->next;
        free(delete_pointer);
        delete_pointer=temp_next;
    }

    free(list);

}
