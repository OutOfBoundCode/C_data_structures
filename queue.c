#include <stdlib.h>
#include "queue.h"
#include "linked_list.h"

queue* create_queue(llcpy cpy, llfree_element free_element, llcompare compare){
    queue* qu = malloc(sizeof(queue));

    if (qu == NULL) return NULL;

    qu->list = create_linked_list(cpy, free_element, compare);
    
    if (qu->list==NULL){
        free(qu);
        return NULL;
    }

    return qu;
}

int enqueue(queue* qu, void* element){
    if (qu == NULL) return -1;

    int success = llappend(qu->list, element);

    return success;
}

void* dequeue(queue* qu){
    if (qu == NULL) return NULL;

    if (qu->list->head==NULL) return NULL; 
      
    void* val = qu->list->cpy(qu->list->head->value);
    
    if (val == NULL) return NULL;
    
    int success = lldelete(qu->list, 0);

    return (success == 0 ? val : NULL);
}

void* queue_front(queue * qu){
    if (qu == NULL) return NULL;

    if (qu->list->head==NULL) return NULL; 

    return qu->list->head->value;
}

int free_queue(queue *qu){
    if (qu == NULL) return -1;

    int free_list_success = free_linked_list(qu->list);
    free(qu);

    return free_list_success;
}
