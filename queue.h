#pragma once
#include "linked_list.h"

/**
 * @brief Queue structure built on top of a linked list.
 */
typedef struct{
    linked_list *list;  /**< Pointer to the underlying linked list storing queue elements */
} queue;

/**
 * @brief Create a new queue.
 * @param cpy a pointer to a copy function that takes void* and return a void pointer to a copy of the memory on success, and return NULL on failure 
 * @param free_element a pointer to a free function that takes a void pointer and free the pointed memory
 * @param compare Function pointer to compare the passed element with the list elements and returning the index when there is a match.
 * @note compare passed function should return 0 on success (the two element match) and -1 on failure.
 * @return Pointer to the newly created queue, or NULL on failure.
 */
queue* create_queue(llcpy cpy, llfree_element free_element, llcompare compare);

/**
 * @brief Enqueue an element at the end of the queue.
 * @param qu Pointer to the queue.
 * @param element Pointer to the element to enqueue.
 * @return 0 on success, -1 on failure.
 */
int enqueue(queue *qu, void* element);

/**
 * @brief Dequeue the front element from the queue.
 * @param qu Pointer to the queue.
 * @note The memory ownership of the dequeued element is transferred to the caller, i.e. The caller is responsible for freeing it.
 * @return Pointer to the dequeued element, or NULL if the queue is empty.
 */
void* dequeue(queue *qu);

/**
 * @brief Peek at the front element of the queue without removing it.
 * @param qu Pointer to the queue.
 * @note No memory ownership is transferred; the queue still owns the memory.
 * @return Pointer to the front element, or NULL if the queue is empty.
 */
void* queue_front(queue *qu);

/**
 * @brief Free the queue and its elements.
 * @param qu Pointer to the queue.
 * @return 0 on success, -1 on failure.
 */
int free_queue(queue *qu);

