#pragma once

#include <sys/types.h>

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
} linked_list;

/**
 * @brief Create a new node.
 * @param element Pointer to the data to store in the node.
 * @return Pointer to the newly created node, -1 on failure.
 */
node *create_node(void* element);

/**
 * @brief Create an empty linked list.
 * @return Pointer to the newly created linked list, -1 failure.
 */
linked_list *create_linked_list();

/**
 * @brief Free the linked list and its nodes.
 * @param list Pointer to the linked list.
 * @param free_element Function pointer to free the data pointed by the value pointer in each node (can be NULL).
 * @note if the memory pointed by the pointers in the linked list is owned by the list, then the user should pass a free_element to free the memory pointed by each node in the list (or it will cause a memory-leak), but if the memory isn't owned by the list (e.g. data stored in constant section or in the stack), the user should pass NULL so the list doesn't try to free that memory.
 * @return 0 on success, -1 on failure.
 */
int free_linked_list(linked_list *list, void (*free_element)(void*));

/**
 * @brief Get the index of an element in the list.
 * @param list Pointer to the linked list.
 * @param element Pointer to the element to find.
 * @param compare Function pointer to compare the passed element with the list elements and returning the index when there is a match.
 * @note compare passed function should return 0 on success (the two element match) and -1 on failure.
 * @return Index of the element, or -1 if not found.
 */
ssize_t llget_index(const linked_list *list, void* element, int (*compare) (void*, void*));

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
 * @param free_element Function pointer to free the old element (can be NULL).
 * @note since the set function would make the value pointer holds the new element address the old element will have no pointer for it. thus it needs to be freed if owned by the list, otherwise the user pass null.
 * @return 0 on success, -1 on failure.
 */
int llset(linked_list *list, ssize_t index, void* element, void (*free_element) (void*));

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
 * @param free_element Function pointer to free the element (can be NULL).
 * @note memory ownership rules in free_list apply here.
 * @return 0 on success, -1 on failure.
 */
int llpop(linked_list *list, void (*free_element)(void*));

/**
 * @brief Delete the element at a specific index.
 * @param list Pointer to the linked list.
 * @param index Index of the element to delete.
 * @param free_element Function pointer to free the element (can be NULL).
 * @note memory ownership rules in free_list apply here.
 * @return 0 on success, -1 on failure.
 */
int lldelete(linked_list *list, ssize_t index, void (*free_element)(void*));

