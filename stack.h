#pragma once

typedef struct{
    array_list *arr;
} stack;

stack* create_stack(ssize_t init_size);

int stack_push(stack *stck, void* element);

void* stack_pop(stack *stck);

void* stack_peek(stack *stck);

int free_stack(stack *stck, void (*free_element) (void*));
