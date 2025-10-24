#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "linked_list.h"

static node* create_node(void* element){
    if (element == NULL) return NULL;

    node* n = malloc(sizeof(node));
    
    if (n == NULL) return NULL;
    
    n->value = element;
    n->next = NULL;
    
    return n;
}

linked_list* create_linked_list(llcpy cpy, llfree_element free_element, llcompare compare){
    linked_list* list = malloc(sizeof(linked_list));
    
    if (list == NULL) return NULL;
    if (free_element == NULL || cpy == NULL || compare == NULL){
        free(list);
        return NULL;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->cpy = cpy;
    list->free_element = free_element;
    list->compare = compare;
    return list;
}

int free_linked_list(linked_list* list){
    if (list == NULL) return -1;
    
    node* delete_pointer = list->head;
    node* temp_next; 
    
    while (delete_pointer != NULL){
        temp_next = delete_pointer->next;
        list->free_element(delete_pointer->value);
        free(delete_pointer);
        delete_pointer = temp_next;
    }

    free(list);

    return 0;
}

ssize_t llget_index(const linked_list* list, void* element) { 
    if (list == NULL || element == NULL) return -1;
    
    node* current = list->head;
    ssize_t current_index = 0;
    
    while (current != NULL) {
        if (list->compare(current->value, element) == 0) return current_index;

        current = current->next;
        current_index++;
    }

    return -1;
}

void llprint(const linked_list* list, void (*print_node) (void*)){
    if (list == NULL || list->length == 0 || print_node == NULL) {
        printf("[]\n");
        return;
    }
    
    node* current = list->head;
    
    printf("[");
    
    while (current != NULL) {
        print_node(current->value);
        current = current->next;
        printf(", ");
    }
    
    printf("\b\b]\n");
}

void llreverse(linked_list* list){
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

node* llget_node(const linked_list* list, ssize_t index){
    if (list == NULL) return NULL;
    if (index < 0) return NULL;
    if (index >= list->length) return NULL;
    
    node* current = list->head;
    ssize_t current_index = 0;

     while (current != NULL && current_index < index) {
        current = current->next;
        current_index++;
    }

    return current;
}

void* llget(const linked_list *list, ssize_t index){
    if (list == NULL || index < 0 || index >= list->length) return NULL;
    node* nd = llget_node(list, index);

    if (nd == NULL) return NULL;

    return nd->value;
}

int llset(linked_list *list, ssize_t index, void* element){
    if (list == NULL || element == NULL || index < 0 || index >= list->length) return -1;

    node* nd = llget_node(list, index);

    void* element_copy = list->cpy(element);

    if (element_copy == NULL) return -1;

    if (nd == NULL) return -1;

    list->free_element(nd->value);

    nd->value = element_copy;

    return 0;
}

int llappend(linked_list*  list, void* element){
    if (list == NULL || element == NULL) return -1;
    return lladd(list, list->length, element);
}


int lladd(linked_list*  list, ssize_t index, void* element){
    if (list == NULL || element == NULL) return -1;

    if (index < 0 || index > list->length) return -1;

    void* element_copy = list->cpy(element);

    if (element_copy == NULL) return -1;
    
    node* newnode = create_node(element_copy);

    if (newnode == NULL) return -1;

    if (list->length == 0){
        list->head = newnode;
        list->tail = newnode;
        list->length++;   
        
        return 0;
    }

    if (index == 0) {
        newnode->next = list->head;
        list->head = newnode;
        list->length++;
       
        return 0;
    }
   
    if (index == list->length){
        list->tail->next = newnode;
        list->tail = newnode;
        list->length++;
        
        return 0;
    }

    node* prenode = llget_node(list, index-1);
    
    if (prenode == NULL) return -1;
    
    node* postnode = prenode->next;
    
    prenode->next = newnode; 
    newnode->next = postnode;
    
    list->length++;
    
    return 0;
}

int llpop(linked_list*  list){
    if (list == NULL) return -1;
    return lldelete(list, list->length-1);
}

int lldelete(linked_list*  list, ssize_t index){
    if (list == NULL) return -1;
   
    if (index < 0 || index >= list->length) return -1;
    
    // check if the list one element
    if (list->length == 1 && index == 0) {
        list->free_element(list->head->value);
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->length--;
        return 0;
    }

    // handle head deletion
    if (index == 0){
        node* oldhead = list->head;
        list->head = list->head->next;
        list->free_element(oldhead->value);
        free(oldhead);
        list->length--;
        return 0;
    }
    
    // handle tail deletion
    if (index == list->length-1){
        node* old_tail = list->tail; 
        node* pretail = llget_node(list, list->length-2); // lengt-2 to get the pre-tail node
        if (pretail==NULL) return -1;
        list->tail = pretail;
        list->tail->next = NULL; 
        list->free_element(old_tail->value);
        free(old_tail);
        list->length--;
        return 0;
    }

    node* prenode = llget_node(list,index-1);
    
    if (prenode == NULL) return -1;
      
    node* deleted_node = prenode->next;
    node* postnode = prenode->next->next;
    
    prenode->next = postnode;
    list->free_element(deleted_node->value);
    free(deleted_node);
    list->length--;

    return 0;
}
