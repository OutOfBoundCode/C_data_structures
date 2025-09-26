#include <stdlib.h>
#include <string.h>
#include "array_list.h"
#include "stack.h"

stack* create_stack(ssize_t init_size){
    stack* stck = malloc(sizeof(stack));
    
    if (stck == NULL) return NULL;

    stck->arr = create_array_list(init_size);

    if (stck->arr == NULL) {
        free(stck);
        return NULL;
    }
    
    return stck;
}

int stack_push(stack* stck, void* element){
    if (stck == NULL) return -1;

    int success = append_element(stck->arr, element);
    
    return success;
}

void* stack_pop(stack* stck){
    if (stck == NULL || stck->arr->length <= 0) return NULL;

    return get_element(stck->arr, --stck->arr->length);
}

void* stack_peek(stack* stck){
    if (stck == NULL || stck->arr->length <= 0) return NULL;
   
    return get_element(stck->arr, stck->arr->length - 1);
}

int free_stack(stack *stck, void (*free_element) (void*)){
    if (stck==NULL) return -1;

    free_array_list(stck->arr, free_element);
    free(stck);

    return 0;
}
