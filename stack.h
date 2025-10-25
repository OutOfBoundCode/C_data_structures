#pragma once

#include <sys/types.h>
#include "array_list.h"

/**
 * @brief Stack structure built on top of an array list.
 */
typedef struct{
    array_list *arr;    /**< pointer to underlying array list storing stack elements */
} stack;

/**
 * @brief Create a new stack with a specified initial size.
 * @param init_size Initial capacity of the stack.
 * @note if the init_size <= 0, the stack size would be 10 (the default for the underlying array list).
 * @param cpy a pointer to a copy function that takes void* and return a void pointer to a copy of the memory on success, and return NULL on failure 
 * @param free_element a pointer to a free function that takes a void pointer and free the pointed memory
 * @param compare Function pointer to compare the passed element with the list elements and returning the index when there is a match.
 * @note compare passed function should return 0 on success (the two element match) and -1 on failure.
 * @return Pointer to the newly created stack, or NULL on failure.
 */
stack* create_stack(ssize_t init_size, alcpy cpy, alfree_element free_element, alcompare compare);

/**
 * @brief Push an element onto the top of the stack.
 * @param stck Pointer to the stack.
 * @param element Pointer to the element to push.
 * @return 0 on success, -1 on failure.
 */
int stack_push(stack *stck, void* element);

/**
 * @brief Pop the top element from the stack.
 * @param stck Pointer to the stack.
 * @note The memory ownership of the popped element is transfered to the caller, i.e. the caller is responsible for freeing the returned element.
 * @return Pointer to the popped element, or NULL if the stack is empty.
 */
void* stack_pop(stack *stck);

/**
 * @brief Peek at the top element of the stack without removing it.
 * @param stck Pointer to the stack.
 * @note no memory ownership gets transfered here, the stack still owns the memory.
 * @return Pointer to the top element, or NULL if the stack is empty.
 */
void* stack_peek(stack *stck);

/**
 * @brief Free the stack and its elements.
 * @param stck Pointer to the stack.
 * @param free_element Function pointer to free the elements (can be NULL).
 * @return 0 on success, -1 on failure.
 */
int free_stack(stack *stck);

