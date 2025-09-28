#include <stdlib.h>
#include "queue.h"
#include "linked_list.h"

queue* create_queue(){
    queue* qu = malloc(sizeof(queue));

    if (qu == NULL) return NULL;

    qu->list = create_linked_list();
    
    if (qu->list==NULL){
        free(qu);
        return NULL;
    }

    return qu;
}

int enqueue(queue* qu, void* element){
    if (qu == NULL) return -1;

    int success = append_node(qu->list, element);

    return success;
}

void* dequeue(queue* qu){
    if (qu == NULL) return NULL;

    if (qu->list->head==NULL) return NULL; 

    void* val = qu->list->head->value;

    int success = delete_node(qu->list, 0, NULL);

    return (success == 0 ? val : NULL);
}

void* queue_front(queue * qu){
    if (qu == NULL) return NULL;

    if (qu->list->head==NULL) return NULL; 

    return qu->list->head->value;
}

int free_queue (queue *qu, void (*free_element) (void*)){
    if (qu == NULL) return -1;

    int free_list_success = free_linked_list(qu->list, free_element);
    free(qu);

    return free_list_success;
}
