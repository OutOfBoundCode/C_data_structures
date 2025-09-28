#pragma once
#include "linked_list.h"

typedef struct{
    linked_list *list;
} queue;

queue* create_queue();

int enqueue(queue *qu, void* element);

void* dequeue(queue *qu);

void* queue_front(queue *qu);

int free_queue(queue *qu, void (*free_element) (void*));
