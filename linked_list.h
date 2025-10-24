#pragma once

#include <sys/types.h>

typedef void* (*llcpy) (void*);
typedef void (*llfree_element) (void*); 
typedef int (*llcompare) (void*, void*); 

/**
 * @brief Node structure for linked list.
 */
struct node {
    void* value;         /**< Pointer to the data stored in the node */
    struct node* next;   /**< Pointer to the next node in the list */
};

typedef struct node node;

/**
 * @brief Linked list structure.
 */
typedef struct linked_list {
    node* head;          /**< Pointer to the first node */
    node* tail;          /**< Pointer to the last node */
    ssize_t length;      /**< Number of elements in the list */
    llcpy cpy;  // a pointer to a copy function that takes a void pointer and return a pointer of the copy of the memory
    llfree_element free_element; // a pointer to a free function that takes a void pointer and free the pointed to memory
    llcompare compare;  // a pointer to compare function that takes two element and return 0 on success (the two element match) and -1 on failure.

} linked_list;

/**
 * @brief Create an empty linked list.
 * @param cpy a pointer to a copy function that takes void* and return a void pointer to a copy of the memory on success, and return NULL on failure 
 * @param free_element a pointer to a free function that takes a void pointer and free the pointed memory
 * @param compare Function pointer to compare the passed element with the list elements and returning the index when there is a match.
 * @note compare passed function should return 0 on success (the two element match) and -1 on failure.
 * @return Pointer to the newly created linked list, -1 failure.
 */
linked_list *create_linked_list(llcpy cpy, llfree_element free_element, llcompare compare);

/**
 * @brief Free the linked list and its nodes.
 * @param list Pointer to the linked list.
 * @return 0 on success, -1 on failure.
 */
int free_linked_list(linked_list *list);

/**
 * @brief Get the index of an element in the list.
 * @param list Pointer to the linked list.
 * @param element Pointer to the element to find.
 * @return Index of the element, or -1 if not found.
 */
ssize_t llget_index(const linked_list *list, void* element);

/**
 * @brief Print the linked list.
 * @param list Pointer to the linked list.
 * @param print_node Function pointer to print the data pointed by the node value pointer.
 */
void llprint(const linked_list *list, void (*print_node) (void*));

/**
 * @brief Reverse the linked list in place.
 * @param list Pointer to the linked list.
 */
void llreverse(linked_list *list);

/**
 * @brief Get the node at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index of the node to retrieve.
 * @return Pointer to the node, or NULL if index is out of range.
 */
node *llget_node(const linked_list *list, ssize_t index);

/**
 * @brief Get the element at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index of the element to retrieve.
 * @return Pointer to the element, or NULL if index is out of range.
 */
void *llget(const linked_list *list, ssize_t index);

/**
 * @brief Set the element at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index of the element to set.
 * @param element Pointer to the new element.
 * @return 0 on success, -1 on failure.
 */
int llset(linked_list *list, ssize_t index, void* element);

/**
 * @brief Append an element to the end of the list.
 * @param list Pointer to the linked list.
 * @param element Pointer to the element to append.
 * @return 0 on success, -1 on failure.
 */
int llappend(linked_list *list, void* element);

/**
 * @brief Add an element at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index at which to insert the element.
 * @param element Pointer to the element to add.
 * @return 0 on success, -1 on failure.
 */
int lladd(linked_list *list, ssize_t index, void* element);

/**
 * @brief Remove the last element from the list.
 * @param list Pointer to the linked list.
 * @return 0 on success, -1 on failure.
 */
int llpop(linked_list *list);

/**
 * @brief Delete the element at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index of the element to delete.
 * @return 0 on success, -1 on failure.
 */
int lldelete(linked_list *list, ssize_t index);
