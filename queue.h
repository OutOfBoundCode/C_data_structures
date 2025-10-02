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
 * @return Pointer to the newly created queue, or NULL on failure.
 */
queue* create_queue();

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
 * @note The memory ownership of the dequeued element is transferred (if it is owned by the queue) to the caller.
 *       The caller is responsible for freeing it if needed.
 * @return Pointer to the dequeued element, or NULL if the queue is empty.
 */
void* dequeue(queue *qu);

/**
 * @brief Peek at the front element of the queue without removing it.
 * @param qu Pointer to the queue.
 * @note No memory ownership is transferred; the queue still owns the element.
 * @return Pointer to the front element, or NULL if the queue is empty.
 */
void* queue_front(queue *qu);

/**
 * @brief Free the queue and its elements.
 * @param qu Pointer to the queue.
 * @param free_element Function pointer to free the elements (can be NULL).
 * @note If the queue owns the memory of its elements, pass a valid free_element function; 
 *       otherwise, pass NULL to avoid freeing memory not owned by the queue.
 * @return 0 on success, -1 on failure.
 */
int free_queue(queue *qu, void (*free_element) (void*));

